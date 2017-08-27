/*
  Copyright (c) 2013 Qualcomm Atheros, Inc..

  All Rights Reserved.
  Qualcomm Atheros Confidential and Proprietary.
*/

'use strict';

/*
 * Ozker library
 *
 * Check out the example app to see how to use this thing.
 */

/*
 * Services
 */

angular.module('ozker', ['poll', 'rate','stats'])

  .config(['$httpProvider', function($httpProvider) {
      //initialize get if not there
      if (!$httpProvider.defaults.headers.get) {
          $httpProvider.defaults.headers.get = {};
      }
      //disable IE ajax request caching
      $httpProvider.defaults.headers.get['If-Modified-Since'] = '0';
  }])

  .constant('url', "/cgi-bin/ozker")

  .service("ozker", ['$q', 'url', 'polling', 'router', 'nodes', 'flows', 'stats', function($q, url, polling, router, nodes, flows, stats) {
    var self=this;
    self.router = router;
    self.router.url = url + '/api/global';

    self.nodes = nodes;
    self.nodes.url = url + '/api/nodes';

    self.flows = flows;
    self.flows.url = url + '/api/flows';

    self.stats = stats;
    self.stats.urls = [url + '/api/events/flows', url + '/api/policies'];

    self.fetch = function() {
        var defer = $q.defer();
        var fetchFlows, fetchRouter, fetchNodes;
        fetchRouter = self.router.fetch();
        fetchRouter.then(function() {
            fetchFlows = self.flows.fetch();
            fetchNodes = self.nodes.fetch();
            $q.all([fetchFlows, fetchNodes])
            .then(function() {
                defer.resolve();
            })
        });
        return defer.promise;
    }

    self.poller = polling.Poller(1000, self.fetch);

    self.startPolling = function() {
      self.poller.start();
    }

    self.stopPolling = function() {
      self.poller.stop();
    }
  }])

  .service("router", ['$http', 'rateCalculator', function($http, rateCalculator) {
    var self=this;
    self.url = undefined;
    self.model = {traffic: {}};

    self.fetch = function() {
      var promise = $http.get(self.url);
      promise = promise.then(function(response) {
        self.model.timestamp = response.data.epoch;
        self.model.uptime = response.data.uptime;
        self.model.timedelta = (response.data.epoch * 1000) - new Date().getTime();
        return self.model;
      });
      return promise;
    }

    self.upRateCalculator = rateCalculator.RateCalculator();
    self.downRateCalculator = rateCalculator.RateCalculator();
  }])

  .service("nodes", ['$http', 'rateCalculator', 'router', 'flows', function($http, rateCalculator, router, flows) {
    var self=this;
    self.url = undefined;
    self.model = [];
    self.byMAC = {};

    /*
     * Update exising node with new data
     */
    function updateNode(node, data) {
      node.mac = data.Pipeline.mac_addr;
      node.ip = data.Pipeline.ip_addr;
      node.name = data.Pipeline.name;
      if (node.os === undefined) {
        node.os = {};
      }
      node.os.type = data.Pipeline.type;
      node.os.flavor = null;
      node.timestamp = data.Pipeline.epoch;
      node.up = data.Pipeline.up;
      node.down = data.Pipeline.down;
      node.user_priority = data.UI.priority;
      node.detection_finished = data.Pipeline.detection_finished;
      node.static = data.Pipeline.static;

      //try to copy connected if it exists
      if(typeof data.Pipeline.connected != 'undefined')
      {
        node.connected = data.Pipeline.connected;
      }
      else // if not connected = false
      {
        node.connected = "0";
      }

      node.wireless = data.Pipeline.wireless;
      node.epoch = data.Pipeline.epoch;
      node.epoch_last_change = data.Pipeline.epoch_last_change;

      if (node.flows === undefined) {
        node.flows = [];
      }
      if (node.upRateCalculator === undefined) {
        node.upRateCalculator = rateCalculator.RateCalculator();
      }
      if (node.downRateCalculator === undefined) {
        node.downRateCalculator = rateCalculator.RateCalculator();
      }
      node.up_rate = node.upRateCalculator.calculate(node.timestamp, node.up);
      node.down_rate = node.downRateCalculator.calculate(node.timestamp, node.down);
    }

    /*
     * Match a node with it's associated flows
     */
    function matchFlows(node, flows) {
      var i, flow;
      node.flows = flows.byMAC[node.mac];
      for (i in node.flows) {

        if (i == 'indexOf')
             continue;
        flow = node.flows[i];
        flow.node = node;
        node.priority = flow.node_priority;
      }
    }

    self.fetch = function() {
      var i, data, node, currentByMAC;
      var promise = $http.get(self.url);
      promise = promise.then(function(response) {
        self.model = [];
        currentByMAC = {};
        router.model.traffic.up = 0;
        router.model.traffic.down = 0;
        for (i in response.data.nodes) {

          if (i == 'indexOf')
              continue;
          data = response.data.nodes[i];
          node = self.byMAC[data.Pipeline.mac_addr];
          if (node === undefined) {
            node = {};
          }
          updateNode(node, data);
          matchFlows(node, flows);
          currentByMAC[node.mac] = node;
          router.model.traffic.up += node.up;
          router.model.traffic.down += node.down;
          self.model.push(node);
        }
        self.byMAC = currentByMAC;

        // These are for the whole router *not* per node
        router.model.traffic.timestamp = data.Pipeline.epoch;
        router.model.traffic.up_limit = data.Pipeline.up_limit;
        router.model.traffic.down_limit = data.Pipeline.down_limit;
        router.model.traffic.up_rate = router.upRateCalculator.calculate(router.model.traffic.timestamp, router.model.traffic.up);
        router.model.traffic.down_rate = router.downRateCalculator.calculate(router.model.traffic.timestamp, router.model.traffic.down);
        return self.model;
      });
      return promise;
    }

    self.writeDirect = function (_sUrl,_kData, _kCallback, _kError, _bPost, _bAsync)
    {
        var sType = "PUT";
        var bAsync = false;

        if(_bPost == true)
        {
             sType = "POST";
        }

        if(_bAsync == true)
        {
            bAsync = true;
        }

        var data = angular.toJson(_kData);//my_toJSON(_kData);

        $.ajax(
        {
            type: sType,
            async: bAsync,
            url: _sUrl,
            data: data,
            dataType: "json",
            timeout: 2000,
            success: function (json, status, request)
            {
                //-------------------------------------------------------------------
                // Direct notification callback
                if ((_kCallback != undefined) || (_kCallback != null))
                    _kCallback();
            },
            //-------------------------------------------------------------------
            // Handle read errors
            error: function (_kResult, textStatus, errorThrown)
            {
                //-------------------------------------------------------------------
                // Direct error callback
                if (_kError != undefined)
                    _kError(_kResult);
            }
        });
    }

    self.changePriority = function(strUID, nPriority)
    {
       for(var key in self.model)
       {
          if (key == 'indexOf')
              continue;

          if(strUID == self.model[key].mac)
          {
            //create a node to modify with our data
            var node = {
              Pipeline: {
                name: "name",
                down_limit: 0,
                down: 0,
                detection_finished: false,
                up_limit: 0,
                ip_addr: "0.0.0.0",
                up: 0,
                OS_flavor: "Unknown",
                future_up: 0,
                epoch: 0,
                uid: "00:00:00:00:00:00",
                type: "Unknown",
                future_down: 0,
                epoch_last_change: 0,
                mac_addr: "00:00:00:00:00:00",
                HTTP_clients: [
                  {
                  flavor: "Unknown",
                  type: "Unknown"
                  }
                ],
                OS_type: "Unknown"
              },
              UI: {
                priority: 0
              }
            };

            //loop through the nodes and find our guy
            for(var n in self.model)
            {
              if (n == 'indexOf')
                  continue;
              //if we find our mac/uid
              if(self.model[n].mac == strUID)
              {
                //fill in the data structure
                node.Pipeline.name      = self.model[n].name;
                node.Pipeline.uid       = self.model[n].mac;
                node.Pipeline.mac_addr  = self.model[n].mac;
                node.Pipeline.ip_addr   = self.model[n].ip;

                //set the priority
                node.UI.priority = nPriority;

                //exit out
                break;
              }
            }

            //write the data (hAcK: un-hard code this path after it works!)
            self.writeDirect("/cgi-bin/ozker/api/nodes", node,
                function ()
                {
                  console.log("Successfully wrote node: " + node.Pipeline.name);
                },
                function (_kResult) // if we fail this is new data so write it as such
                {
                  console.log("Error writing node: " + node.Pipeline.name);
                }
             );
          }
       }
    }

  }])

  .service("flows", ['$http', 'rateCalculator', 'router', function($http, rateCalculator, router) {
    var self=this;
    self.url = undefined;
    self.model = [];
    self.byMAC = {};
    self.byUID = {};

    /*
     * Update an existing flow with new data
     */
    function updateFlow(flow, data) {
      flow.uid = data.uid;
      flow.mac = data.mac;

      //if we have a valid group use it as the name
      if( typeof data.group != 'undefined' &&
          data.group != null &&
          data.group != "" &&
          data.group != "N/A" &&
          data.group != "n/a")
      {
        flow.name = data.group;
      }
      else
      {
        flow.name = data.name;
      }

      //no matter what record the group and emit names
      flow.emit_name = data.name;
      flow.group = data.group;

      flow.protocol = data.protocol;
      flow.up = data.up_bytes;
      flow.down = data.down_bytes;
      flow.connection_count = data.connections;
      flow.timestamp = data.epoch;
      flow.node_priority = data.node_priority;
      if (flow.policies === undefined) {
        // build policies structure
        flow.policies = {
          up: {
            optimal: {},
            nominal: {}
          },
          down: {
            optimal: {},
            nominal: {}
          }
        };
      }
      flow.policies.type = data.policies.type;
      flow.policies.up.active = data.policies.up_profile_active;
      flow.policies.up.optimal.rate = data.policies.up_profile_optimal.rate;
      flow.policies.up.nominal.rate = data.policies.up_profile_nominal.rate;
      flow.policies.down.active = data.policies.down_profile_active;
      flow.policies.down.optimal.rate = data.policies.down_profile_optimal.rate;
      flow.policies.down.nominal.rate = data.policies.down_profile_nominal.rate;

      updateCurrentRate(flow, data);
    }

    /*
     * Update and calculate the current rate
     */
    function updateCurrentRate(flow, data) {
      if (flow.upRateCalculator === undefined) {
        flow.upRateCalculator = rateCalculator.RateCalculator();
      }
      if (flow.downRateCalculator === undefined) {
        flow.downRateCalculator = rateCalculator.RateCalculator();
      }
      flow.up_rate = flow.upRateCalculator.calculate(flow.timestamp, flow.up);
      flow.down_rate = flow.downRateCalculator.calculate(flow.timestamp, flow.down);
    }

    self.fetch = function() {
      var i, data, flow, currentByUID;
      var promise = $http.get(self.url);
      promise = promise.then(function(response) {
        self.model = [];
        self.byMAC = {};
        currentByUID = {};
        router.model.traffic.up_subscribed = 0;
        router.model.traffic.down_subscribed = 0;
        for (i in response.data.flows) {
          if (i == 'indexOf')
              continue;
          data = response.data.flows[i];
          flow = self.byUID[data.uid];
          if (flow === undefined) {
            flow = {};
          }
          updateFlow(flow, data);
          currentByUID[flow.uid] = flow;
          self.model.push(flow);
          if (self.byMAC[flow.mac] === undefined) {
            self.byMAC[flow.mac] = [];
          }
          self.byMAC[flow.mac].push(flow);
          router.model.traffic.up_subscribed += flow.policies.up.optimal.rate;
          router.model.traffic.down_subscribed += flow.policies.down.optimal.rate;
        }
        self.byUID = currentByUID;
        return self.model;
      });
      return promise;
    }

  }])

/*
 * Filters
 */


  .filter('rate', function() {
    return function(Bps) {
      var bps, humanRate;

      bps = Bps * 8;
      if (bps < 1000) {
        humanRate = bps.toFixed(0) + " bps";
      } else if (bps < 1000000) {
        humanRate = (bps/1000).toFixed(2) + " Kbps";
      } else {
        humanRate = (bps/1000000).toFixed(2) + " Mbps";
      }
      return humanRate;
    }
  })

  .filter('hex', function() {
    return function(myDecimal) {
      return parseInt(myDecimal, 10).toString(16);
    }
  });
