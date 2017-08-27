/*
  Copyright (c) 2013 Qualcomm Atheros, Inc..

  All Rights Reserved.
  Qualcomm Atheros Confidential and Proprietary.
*/

'use strict';

angular.module('ozkerDebug.filters', [])
  .filter('Mbps', function() {
    return function(Bps) {
      return ((Bps * 8)/1000000).toFixed(2);
    };
  })
  .filter('checkIcon', function() {
    function endsWith(string, suffix) {
      return string.indexOf(suffix, string.length - suffix.length) !== -1;
    };

    return function(possibleIcon) {
      if (endsWith(possibleIcon, '.png') || endsWith(possibleIcon, '.jpg')) {
        return possibleIcon;
      } else {
        return '/images/default.png';
      }
    };
  });
