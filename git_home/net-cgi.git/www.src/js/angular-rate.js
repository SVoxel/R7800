/*
  Copyright (c) 2013 Qualcomm Atheros, Inc..

  All Rights Reserved.
  Qualcomm Atheros Confidential and Proprietary.
*/

'use strict';

/*
 * Rate library
 *
 * This is a little library to calculate rate given a timestamp and value
 *
 * ex:
 * rateCalc = rateCalculator.RateCalculator();
 * currentRate = rateCalculator.calculate(timestamp, value);
 *
 * ...where value is usually bytes.  If you specify an interval, the rate
 * will be calculated over that interval.  The default interval is 1 second.
 *
 */

angular.module('rate', [])
  .service("rateCalculator", function() {
    var defaultInterval = 1; // seconds

    this.RateCalculator = function(interval) {
      var self=this;
      var prevTimestamp, prevValue, prevRate;

      if (! interval) {
        interval = defaultInterval;
      }

      var calculate = function(timestamp, value) {
        var timeDelta, rate;
        if (prevTimestamp === undefined) {
          rate = 0;
          prevTimestamp = timestamp;
          prevValue = value;
          prevRate = rate;
        } else {
          timeDelta = timestamp - prevTimestamp;
          if (timeDelta == 0) {
            rate = prevRate;
          } else if (value < prevValue) {
            // WORKAROUND: We occasionally get zeros for the byte count
            // from ozker.  Handle that here.
            if (value == 0) {
              rate = prevRate
              value = prevValue
            } else {
              // handle 32-bit rollover
              rate = ((Math.pow(2, 32) - prevValue) + value) / timeDelta;
            }
          } else {
            rate = (value - prevValue) / timeDelta;
          }
          // WORKAROUND: When flows go away those bytes disappear from
          // the total.  This is a bug in ozker.  Workaround that
          // here.
          // over 1 Gbps?  I don't think so.  Use the prevRate
          if ((rate * 8) / Math.pow(1000, 3) > 1) {
            rate = prevRate;
          }
          prevRate = rate;
          if (timeDelta >= interval) {
            prevTimestamp = timestamp;
            prevValue = value;
          }
        }
        return rate;
      };

      return {
        'calculate': calculate,
      };
    }
  });
