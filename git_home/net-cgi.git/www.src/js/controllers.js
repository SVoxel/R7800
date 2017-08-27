/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
'use strict';

////////////////////////////////////////////////////////////////////////
/*
    Class: Controllers

    Version:   1.0

    This is the angular controllers used for the model by the flux
    system. This hooks up and primes all the data for the GUI

*/
////////////////////////////////////////////////////////////////////////

angular.module('ozkerDebug.controllers', []).
    controller('OzkerCtrl', ['$scope', '$q', 'ozker', '$translate', '$translatePartialLoader', 'deviceIconLoader', 'flowIconLoader',
      function($scope, $q, ozker, $translate, $translatePartialLoader, deviceIconLoader, flowIconLoader) {
        $scope.router = ozker.router;
        $scope.nodes = ozker.nodes;
        $scope.flows = ozker.flows;
        ozker.startPolling();
        $scope.$on('$destroy', ozker.stopPolling);

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

        $scope.pollState = "Running";
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
        $scope.updateNode = function(strUID,nPriority)
        {
            alert("got an update!!");
        }

        $scope.deviceIconLoader = deviceIconLoader;
        $scope.flowIconLoader = flowIconLoader;

        //set the default language
        $scope.language = getLanguage();//can default to any language here (like: 'EN-US')
        $translatePartialLoader.addPart('translate');
        $translatePartialLoader.addPart('flows');

        //if the user changes the language
        $scope.changeLanguage = function() {
          $translate.uses($scope.language);
          $translate.refresh();
        };

        //refresh the language layer
        $translate.refresh();

        //set global access to the translation layer
        $scope.translatePartialLoader = $translatePartialLoader;
        $scope.translate = $translate;
    }]);
