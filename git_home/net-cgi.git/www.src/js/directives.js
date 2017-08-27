/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
'use strict';

////////////////////////////////////////////////////////////////////////
/*
    Class: Directives

    Version:   1.0

    This is the angular directive used for by the flux router
    system. This creates the new html gui components that are used by
    the streamboost router system. The angular system compiles these
    at load time and calls these functions to make each element.

*/
////////////////////////////////////////////////////////////////////////

/*
    Variable: bSetLanguage

    Has a language translation layer been loaded?

    Defaults to false.
*/
var bSetLanguage = false;

angular.module('ozkerDebug.directives', ['stats'])
    ////////////////////////////////////////////////////////////////////////
    /*
        Function: fxNetwork

        This define the fx-network custom html element that is used to
        add a network graph of devices to your gui

        in you html:
         <fx-network></fx-network>

         will create a network graph
        Parameters:
          none.

        Returns:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    .directive('fxNetwork', [function () {
        return {
            restrict: 'E',
            template: '<div id="Display" style="min-width:800px;height:100%;"></div>',
            link: function(scope, element, attrs) {
                //set the global streamboost depent variable
                var g_bIsStreamboost = true;

                //handler function for mouse clicks
                function handler(icon,name,x,y,node,id)
                {
                    var pathname = window.location.pathname;

                    var n=pathname.indexOf("mynetwork");

                    if(n==-1)
                    {
                        pathname = pathname + "/node";
                    }
                    else
                    {
                        pathname = pathname.replace(/mynetwork/g,"node")
                    }

                    //send the browser to the node page
                    //append the id of the clicked on node
                    window.location = pathname+'?nodeid='+id;

                    // window.location = "http://192.168.1.1/Angular/Flux2/index.html#/node?nodeid="+id;
                    // window.reload(true);
                };

                //stash this off for closure access in function below
                var pScope = scope;

                //image loader
                var pDeviceLoader =  function(type)
                {
                    return pScope.deviceIconLoader.load(type);
                };

                //create the flux network class
                scope.netview = new fxNetwork(  "Display",      //div to draw into
                                                handler,        //mouse callback
                                                pDeviceLoader);

                //update your display here
                scope.netview.updateData([]);

                //update your display here
                scope.netview.updateRouter({});


                scope.$watch('nodes.model', function(nodes){
                    //update your display here
                    scope.netview.updateData(nodes);
                },false);

                scope.$watch('router.model.traffic', function(router){
                    //update your display here
                    scope.netview.updateRouter(router);
                },true);
            }
        };
    }])
    ////////////////////////////////////////////////////////////////////////
    /*
        Function: fxNode

        This sets up the custom node html element that is used to examine
        the flows running on a node

        in you html:
         <fx-node></fx-node>

         will create a node view panel
        Parameters:
          none.

        Returns:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    .directive('fxNode', [function () {
        return {
            restrict: 'E',
            template: '<div id="Display" style="min-width:800px;height:100%;margin-top:-25px;"></div>',
            link: function(scope, element, attrs) {
                $.urlParam = function(name){
                    var results = new RegExp('[\\?&]' + name + '=([^&#]*)').exec(window.location.href);

                    if(results !=undefined && results.length>=1)
                    {
                        return results[1];
                    }
                    else
                    {
                        return 0;
                    }
                }
                //get the node to show from the parametrs of your url
                var strNode = $.urlParam("nodeid");

                //stash this off for closure access in function below
                var pScope = scope;

                //image loader
                var pDeviceLoader =  function(type)
                {
                    return pScope.deviceIconLoader.load(type);
                };

                //image loader
                var pFlowLoader =  function(type)
                {
                    return pScope.flowIconLoader.load(type);
                };

                //create the flux node class
                scope.netview = new fxNode( "Display",      //div to display in
                                            null,           //flows,
                                            null,           //devices,
                                            strNode,        //node id to display
                                            pDeviceLoader,  //device image loader function
                                            pFlowLoader);   //flow image loader function

                //update your display here
                scope.netview.updateFlows([]);

                //update your display here
                scope.netview.updateNodes([]);

                scope.$watch('flows.model', function(flows) {

                    //add realname to all the flows
                    for (var i = flows.length - 1; i >= 0; i--) {
                        flows[i].RealName = scope.translate(flows[i].name);
                    };

                    //update your display here
                    scope.netview.updateFlows(flows);
                },false);

                scope.$watch('nodes.model', function(nodes) {

                    //add realname to all the nodes
                    for (var i = nodes.length - 1; i >= 0; i--) {
                        nodes[i].RealName = scope.translate(nodes[i].name);
                    };

                    //update your display here
                    scope.netview.updateNodes(nodes);
                },false);

                //if the translation layer isnt loaded
                if(bSetLanguage == false)
                {
                    //then load all the trans layers
                    scope.translatePartialLoader.addPart('translate');
                    scope.translatePartialLoader.addPart('flows');
                    scope.translate.uses(scope.language);
                    scope.translate.refresh();
                    bSetLanguage = true;
                }
            }
        };
    }])
    ////////////////////////////////////////////////////////////////////////
    /*
        Function: fxPriority

        This sets up the custom priority html element that is used to
        modify the priority of network devices on your router.

        in you html:
         <fx-priority></fx-priority>

         will create a priority config panel
        Parameters:
          none.

        Returns:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    .directive('fxPriority', [function () {
        return {
            restrict: 'E',
            template: '<div id="Display" style="min-width:945px;height:100%;margin-top:-25px;"></div>',
            link: function(scope, element, attrs) {

                //default to hide buttons
                var bShowButtons = true;

                //if the user defines the attribute to show
                if( typeof attrs.showbuttons != undefined &&
                    attrs.showbuttons == "true")
                {
                    bShowButtons = true;
                }

                //stash this off for closure access in function below
                var pScope = scope;

                //change priority
                var pPrioChange =  function(uid,prio)
                {
                    pScope.nodes.changePriority(uid,prio);
                };

                //image loader
                var pDeviceLoader = function(type)
                {
                    return pScope.deviceIconLoader.load(type);
                };

                //create the flux node class
                scope.priority = new fxPriority("Display",          //div to display in
                                                pPrioChange,        //external save routine
                                                bShowButtons,       //show save/cancel buttons = true
                                                attrs.save,         //save click callback
                                                attrs.cancel,       //cancel click callback
                                                pDeviceLoader);

                scope.$watch('nodes.model', function(nodes){

                    //update your display here
                    scope.priority.updateData(nodes);

                },false);
            }
        };
    }])
    ////////////////////////////////////////////////////////////////////////
    /*
        Function: fxTopByTime

        This sets up the custom graph html element that is used to
        display stats about devices and nodes on your network.

        in you html:
         <fx-top-by-time></fx-top-by-time>

         will create a graph.
        Parameters:
          none.

        Returns:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    .directive('fxTopByTime',['$http', 'stats', function ($http, stats) {
        return {
            restrict: 'E',
            template:
                '<div class="chartOptions" style="float: right;margin-top: -64px;margin-right:64px;"">'+
                '  <div>'+
                '    <select class="dateRangeFilter">'+
                '      <option value="day">Last Day</option>'+
                '      <option value="week">Last Week</option>'+
                '      <option value="month">Last Month</option>'+
                '    </select>'+
                '  </div>'+
                '  <div>'+
                '    <select class="nodeFilter">'+
                '    </select>'+
                '  </div>'+
                '</div>'+
                '<div id="chart_div_withoptions" style="margin-left:auto;margin-right:auto;margin-top:64px;"></div>',
            link: function(scope, element, attrs) {

                //stash this off for closure access in function below
                var pScope = scope;

                //if the translation layer isnt loaded
                if(bSetLanguage == false)
                {
                    //then load all the trans layers
                    scope.translatePartialLoader.addPart('translate');
                    scope.translatePartialLoader.addPart('flows');
                    scope.translate.uses(scope.language);
                    scope.translate.refresh();
                    bSetLanguage = true;
                }

                //this handles translation for the chart
                function transFunction(val)
                {
                    return pScope.translate(val);
                };

                function setupNodeFilter(callback) {
                  $http.get('/cgi-bin/ozker/api/nodes').then(function(data1) {
                    var data = data1.data;
                    $('.nodeFilter').html('<option value="">All LAN Hosts</option>');
                    $.each(data['nodes'], function(i, node) {
                        var text;
                        if (node['Pipeline']['name'] != null) {
                            text = node['Pipeline']['name'];
                        } else {
                            text = node['Pipeline']['ip_addr'];
                        }
                        var value = node['Pipeline']['mac_addr'];
                        $('.nodeFilter').append('<option value="' + value + '">' + text + '</option>');
                    });
                    callback();
                  });
                }

                function handleFilterChange() {
                  var dateRange = $('.dateRangeFilter option:selected');
                  var node = $('.nodeFilter option:selected');
                  var msecs_per_day = 24 * 60 * 60 * 1000;
                  var msecs = {
                    day: msecs_per_day,
                    week: 7 * msecs_per_day,
                    month: 30 * msecs_per_day
                  };

                  var end = new Date(getCurrentTime());
                  var start = new Date(end - msecs[dateRange.val()]);
            //      $('.chartTitle').text("Top 5 Flows In The " + dateRange.text() + " For " + node.text());
                  graphTopByTime(start, end, node.val());
                }

                $('.dateRangeFilter').change(handleFilterChange);
                $('.nodeFilter').change(handleFilterChange);
                setupNodeFilter(function() {
                    $('.dateRangeFilter').trigger('change');
                });

                function graphTopByTime(start,end,node) {
                    stats.getFlowsTime(start,end,node).then(function(topData) {
                        if (scope.chart) delete scope.chart;
                        scope.chart = new fxTopByTime("#chart_div_withoptions", topData, stats,transFunction);
                        $(".ng-scope").css("min-width","1150px");
                    });
                }
            }

        };
    }])
    ////////////////////////////////////////////////////////////////////////
    /*
        Function: fxTopDownloads

        This sets up the custom graph html element that is used to
        display stats about devices and nodes on your network.

        in you html:
         <fx-top-downloads></fx-top-downloads>

         will create a graph.
        Parameters:
          none.

        Returns:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    .directive('fxTopDownloads', ['stats', function (stats) {
        return {
            restrict: 'E',
            template: '<div id="chart_div" style="margin-left: auto;margin-right: auto;"></div>',
            link: function(scope, element, attrs) {

                //stash this off for closure access in function below
                var pScope = scope;

                //if the translation layer isnt loaded
                if(bSetLanguage == false)
                {
                    //then load all the trans layers
                    scope.translatePartialLoader.addPart('translate');
                    scope.translatePartialLoader.addPart('flows');
                    scope.translate.uses(scope.language);
                    scope.translate.refresh();
                    bSetLanguage = true;
                }

                //this handles translation for the chart
                function transFunction(val)
                {
                    return pScope.translate(val);
                };

                stats.getFlowsBytesDown().then(function(topData) {
                    scope.chart = new fxDownloadPie("#chart_div", topData, stats, transFunction);
                });
            }
        };
    }])
    ////////////////////////////////////////////////////////////////////////
    /*
        Function: fxAllEvents

        This sets up the custom graph html element that is used to
        display stats about devices and nodes on your network.

        in you html:
         <fx-all-events></fx-all-events>

         will create a graph.
        Parameters:
          none.

        Returns:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
    // this graph is not supported anymore!
    .directive('fxAllEvents', ['stats', function (stats) {
        return {
            restrict: 'E',
            template: '<div id="Display" style="min-height:600px;min-width:800px;height:90%;width:100%;margin-top:64px;"></div>',
            link: function(scope, element, attrs) {
                //create the stats object
                //var stats = new fxStats();

                //get the top flows by download
                stats.getFlowsEvents().then(function(allData) {

                    //draw the gant chart tell it to draw in the below div
                    scope.gantt = new fxGantt("#chart_div",allData);
                });
            }
        };
    }]);