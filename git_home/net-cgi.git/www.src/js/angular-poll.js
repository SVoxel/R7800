/*
  Copyright (c) 2013 Qualcomm Atheros, Inc..

  All Rights Reserved.
  Qualcomm Atheros Confidential and Proprietary.
*/

'use strict';

/*
 * Poll library
 *
 * This is a little library to poll stuff
 *
 * ex:
 * poller = polling.Poller(2000, func);
 * poller.start();
 *
 * ...where func is a function that returns an angular promise.
 */

angular.module('poll', [])
  .service("polling", ['$timeout', function($timeout) {
    var defaultInterval = 2000; // milliseconds

    this.Poller = function(interval, func, pollHandler) {
      var _isPolling = false;

      function poll() {
        return $timeout(function() {
          if (_isPolling) {
            func().then(runAgain);
          }
        }, interval);
      }

      function runAgain(result) {
        if (pollHandler !== undefined) {
          pollHandler(result).then(function() { poll(interval) });
        } else {
          poll(interval);
        }
      };

      var isPolling = function() {
        return _isPolling;
      }

      var start = function() {
        if (! _isPolling) {
          if (! interval) {
            interval = defaultInterval;
          }
          _isPolling = true;
          func().then(runAgain);
        }
      }

      var stop = function() {
        if (_isPolling) {
          _isPolling = false;
        }
      }

      var onPoll = function(handler) {
        pollHandler = handler;
      };

      return {
        'isPolling': isPolling,
        'start': start,
        'stop': stop,
        'onPoll': onPoll,
      };
    };
  }]);
