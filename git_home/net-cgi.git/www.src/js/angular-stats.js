/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
angular.module('stats', ['ozker'])
    .service('stats',['$http', '$q', 'router', function($http,$q, router) {
        var self = this;
        var routerEndTime;
        var vData;
        var policycache = {},
            flowsByName = {},
            flows = [];

        var filterDateStart, filterDateEnd, vMacFilters = [];

        var g_routerDelta = 0;

        this.getFlowsTime = function(start,end,nodes) {
            return self.fetch(self.urls,start,end,nodes).then(function() { return getByTime(vData);});
        }

        this.getFlowsBytesDown = function() {
            return self.fetch(self.urls).then(function() { return getBytesDown(vData); });
        }

        this.getFlowsBytesUp = function() {
            return self.fetch(self.urls).then(function() { return getBytesUp(vData); });
        }

        this.getFlowsEvents = function() {
            return self.fetch(self.urls).then(function() { return getEvents(vData); });
        }

        //convert bytes to Megabits 2 decimal places
        function B2Megabytes(val) {
            var nReturn = 0;

            nReturn = ((val / g_nMegSize) / g_nMegSize);

            if(nReturn < 0)
            {
                nReturn = 0;
            }

            return nReturn;
        }

        function getEvents(vData) {
            var vReturn = [];
            // 4 strata?!
            vReturn[0] = [];
            vReturn[1] = [];
            vReturn[2] = [];
            vReturn[3] = [];
            for(var i in vData) {
                var flowobj = vData[i];
                for (j = 0; j < flowobj.flows.length; j++) {
                    var o = flowobj.flows[j];
                    if (!o.close || !o.policy) continue; //unclosed or unknown
                    if (!isFilteredOut(o)) {
                        //add the guy to the end of our return
                        vReturn[0].push([o.open.time,i,o.close.time,i]);
                    }
                }
            }
            return vReturn;
        }

        function getByTime(vData)
        {
            var vReturn = [];

            vReturn.push([_t("Flow Name"), _t("Minutes")]);

            //walk the list and get the times
            for(var i in vData)
            {
                var strName = i;

                // pre-calculated by adding all times for the flowName
                var nTime = (vData[i].totalTime/1000)/60; // these are all in minutes

                //make our list
                vReturn.push([strName,nTime]);
            }

            return vReturn;
        }


        function getBytesDown(vData) {
            var vReturn = [];

            //vReturn.push(["Flow Name", "Bytes Downloaded","Tool Tip"]);

            //walk the list
            for(var i in vData)
            {
                //get name
                var strName = i;
                var nDL = 0;

                //if we have a valid download (don't show zeros)
                if(vData[i].nDownload)
                {
                    nDL = vData[i].nDownload;
                    //make our list
                    vReturn.push([strName,B2Megabytes(nDL)]);
                }
            }

            return vReturn;
        }

        function getBytesUp(vData) {
            var vReturn = [];

            //vReturn.push(["Flow Name", "Bytes Downloaded","Tool Tip"]);

            //walk the list
            for(var i in vData)
            {
                //get name
                var strName = i;
                var nDL = 0;

                //if we have a valid download (don't show zeros)
                if(vData[i].nDownload)
                {
                    nDL = vData[i].nUpload;
                    //make our list
                    vReturn.push([strName,B2Megabytes(nDL)]);
                }
            }

            return vReturn;
        }


        function isFilteredOut(dat) {
            if (filterDateStart && (dat.open.time < filterDateStart)) return true;
            if (filterDateEnd && (dat.close.time > filterDateEnd)) return true;

            if (vMacFilters && vMacFilters.length) {
                fmac = vMacFilters;
                if (fmac != dat.open.mac) return true;
            }

            return false;
        }

        var saveflows = {};

        function isRawFilteredOut(ev) {
            if (saveflows[ev.uuid]) { // mac close
                // if enddate is > self.dateEnd then truncate?
                // if startdate is < self.dateStart then trucate?
                delete saveflows[ev.uuid];
                return false;
            }
            var dattime = new Date(parseInt(ev.time)*1000 + g_routerDelta);
            if (filterDateStart && (dattime < filterDateStart)) return true;
            if (filterDateEnd && (dattime > filterDateEnd)) return true;
/*
            if (vMacFilters && vMacFilters.length) {
                var fmac;
                for (var f in vMacFilters) {
                    fmac = vMacFilters[f];
                    if (fmac == ev.mac) {
                        saveflows[uuid] = true;
                        break;
                    }
                }
                if (fmac != datmac) return true;
            }
*/
            return false;
        }

        // for a given flow name, there will be multiple "event pairs", compute a total time and bytes
        // for each flow type
        this.combineEvents = function(byflow) {
            for (var i in byflow) {
                var flowobj = byflow[i]; // this is a bunch of "opens"
                flowobj.nDownload = 0;
                flowobj.nUpload = 0;
                flowobj.totalTime = 0;
                flowobj.times = [];
                for (j = 0; j < flowobj.flows.length; j++) {
                    var o = flowobj.flows[j];
                    if (!o.close || !o.policy) continue; //unclosed or unknown
                    if (!isFilteredOut(o)) {
                        flowobj.nDownload += o.close.details.rx_bytes;
                        flowobj.nUpload += o.close.details.tx_bytes;
                        flowobj.times.push({timeOpen: o.open.time, timeClose: o.close.time});
                        flowobj.totalTime += o.close.time - o.open.time;
                    }
                }
            }
            return byflow;
        }

        function getPolicyCache(policyUrl) {
            if (policycache.length) return true;
            else {
                return $http.get(policyUrl)
                .success(function(data) {
                    for (var id in data) {
                        policycache[id] = data[id];
                    }
                });
            }
        }

        var dataUrl;
        var policyUrl;

        this.fetch = function(urls, start,end,nodes) {
            return router.fetch()
            .then(function() {
                return getPolicyCache(urls[1]);
            })
            .then(function() {
                dataUrl = urls[0];
                g_routerDelta = 0;//old way: router.model.timedelta;
                return self.getData(start,end,nodes);
            })
            .then(function(byflows) {
                //compute aggregates by flow
                var aggData = self.combineEvents(byflows);
                vData = aggData;
                return aggData; // Q(aggData)
            });
        }

        function lastpath(s) {
            var slash = s.lastIndexOf("/");
            if (slash == -1) {
                slash = s.lastIndexOf(":");
            }
            if (slash == -1) return "unknown";
            else return s.substr(slash+1);
        }


        var byflows = {};

        //current router time
        var currentTime = null;
        //handy 30 day variable
        var thirtydays = 30*24*60*60*1000;

        this.validTime = function(test)
        {
            var rt = false;

            var current = currentTime.getTime();
            //get the diff between now and this data
            var diff =  current - test;

            //if the time is invalid set it to 30 days minus current router time
            if( diff > 0 &&
                diff < thirtydays)
            {
                rt = true;
            }

            return rt;
        }

        //
        // This method should fill in the vData member variable
        // converted and returned by the fxStats binding methods
        //
        this.getData = function(start, end, vFilter) {
            filterDateStart = start;
            filterDateEnd = end;
            vMacFilters = (vFilter && vFilter.length) ? vFilter : null;

            var deferred = $q.defer()
            var strData = "";

            var sDate = new Date();//$( "#topFrom" ).datepicker( "getDate" );
            var eDate = new Date();//$( "#topTo" ).datepicker( "getDate" );
            byflows = {};

            //cache the current router time
            currentTime = getCurrentTime();

            // angular will fail to parse the crappy JSON
            $http.get(dataUrl,{timeout:60000,
                transformResponse: function(data, headersGetter) {
                    return data;
                }})
                .error(function(data, status, headers, config) {
                    deferred.reject(_t("Unable to get requested data. Please check your connection to your router."));
                })
                .success(function(datax, status, headers, config) {
                    //var data2 = datax.replace(/:([a-z_]+)/g,':"$1"');

                    // this filters the data to a more manageable amount (for parsing)
                    // and also handles the non-quoted strings in some of the 5-tuple data
                    var data2 = datax.replace(/{"5-tuple".*}\s*,/g,'');

                    var data = JSON.parse(data2);

                    var opens = {};

                    for (var i = 0; i < data.events.length; i++) {
                        var flow = data.events[i];
                        var nid = flow.uuid;
                        if (flow.event == "open") {
                            if (opens[nid]) {
                                // error "double id"!!!!
                                deferred.reject(_t("error double open"));
                            }
                            else {
                                var nTime = parseInt(flow.time);
                                nTime *= 1000;
                                nTime += g_routerDelta;

                                //if this is a valid time range
                                if(self.validTime(nTime))
                                {
                                    flow.time = new Date(nTime);
                                    opens[nid] = { open: flow };
                                    /* temporary */
                                    if (policycache[flow.details.policy_id]) {
                                        opens[nid].policy = policycache[flow.details.policy_id];
                                    }
                                    else {
                                        opens[nid].policy = {emit: lastpath(flow.details.policy_id)};
                                    }
                                }
                            }
                        }
                        else if ((flow.event == "close") || (flow.event == "milestone")) {
                            if (!opens[nid]) {
                                console.log(_t("unmatched close/milestone"));
                            }
                            else {
                                var nTime = parseInt(flow.time);
                                nTime *= 1000;
                                nTime += g_routerDelta;
                                flow.time = new Date(nTime);


                                //if this is a valid time range
                                if(self.validTime(nTime))
                                {
                                    flow.details.rx_bytes = parseInt(flow.details.rx_bytes);
                                    flow.details.tx_bytes = parseInt(flow.details.tx_bytes);
                                    opens[nid].close = flow;
                                }
                            }
                        }
                        else if (flow.event == "oversub") {
                            //TODO: need to know what this record looks like!
                        }
                    }

                    var now = new Date().getTime();

                    //make sure by flows is empty!
                    byflows = {};

                    // now see what's left in "opens"
                    // and construct our return data
                    for (var o in opens) {
                        if (!opens[o].close) {
                            opens[o].close = {
                                time: now,
                                details: {tx_bytes: 0, rx_bytes: 0}
                            };
                        }

                        //if we don't have this is by flows
                        if(typeof byflows[opens[o].policy.emit] == 'undefined')
                        {
                            //create our data field
                            byflows[opens[o].policy.emit] = {};

                            //make the flows list
                            byflows[opens[o].policy.emit].flows = [];
                        }

                        //add this open/close set to the correct flow
                        byflows[opens[o].policy.emit].flows.push(opens[o]);
                    }

                    //send this back now that we have it done
                    deferred.resolve(byflows);
                }); // .success

                return deferred.promise;
            }

    }]);