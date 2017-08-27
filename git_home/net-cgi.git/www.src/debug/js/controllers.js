/*
  Copyright (c) 2013 Qualcomm Atheros, Inc..

  All Rights Reserved.
  Qualcomm Atheros Confidential and Proprietary.
*/

'use strict';

/* Controllers */

angular.module('ozkerDebug.controllers', []).
  controller('OzkerCtrl', ['$scope', '$q', 'ozker', '$translate', '$translatePartialLoader', 'deviceIconLoader', 'flowIconLoader', function($scope, $q, ozker, $translate, $translatePartialLoader, deviceIconLoader, flowIconLoader) {

    $scope.router = ozker.router;
    $scope.nodes = ozker.nodes;
    $scope.flows = ozker.flows;

    $scope.activePolicyRateUp = function(flow) {
      if (flow.policies.up.active == 0) {
        return flow.policies.up.optimal.rate;
      } else if (flow.policies.up.active == 1) {
        return flow.policies.up.nominal.rate;
      }
    };

    $scope.activePolicyRateDown = function(flow) {
      if (flow.policies.down.active == 0) {
        return flow.policies.down.optimal.rate;
      } else if (flow.policies.down.active == 1) {
        return flow.policies.down.nominal.rate;
      }
    };
  
    ozker.startPolling();
    $scope.pollState = "Running";
    $scope.$on('$destroy', ozker.stopPolling);
    $scope.togglePolling = function() {
      if (ozker.poller.isPolling()) {
        ozker.poller.stop();
        $scope.pollState = "Stopped";
      } else {
        ozker.poller.start();
        $scope.pollState = "Running";
      }
    };

    $scope.step = function() {
      ozker.fetch();
    };

    $scope.deviceIconLoader = deviceIconLoader;
    $scope.flowIconLoader = flowIconLoader;

    $scope.language = 'EN-US';
    $translatePartialLoader.addPart('translate');
    $translatePartialLoader.addPart('flows');
    $translate.refresh();
    $scope.changeLanguage = function() {
      $translate.uses($scope.language);
    };

  }]);
