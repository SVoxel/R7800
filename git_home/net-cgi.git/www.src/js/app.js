/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
'use strict';

////////////////////////////////////////////////////////////////////////
/*
    Class: App

    Version:   1.0

    This is the angular application used for the model by the flux
    system. This wraps all data coming from ozker.

*/
////////////////////////////////////////////////////////////////////////

// Declare app level module which depends on filters, and services
angular.module('ozkerDebug', ['ngRoute', 'ozkerDebug.controllers', 'ozkerDebug.filters', 'ozker', 'pascalprecht.translate', 'translateFallbackLoader', 'fallbackLoader', 'iconLoader','ozkerDebug.directives']).
  config(['$routeProvider', function($routeProvider) {
        $routeProvider.when('/mynetwork', {templateUrl: 'partials/mynetwork.html', controller: 'OzkerCtrl'});
        $routeProvider.when('/node', {templateUrl: 'partials/nodeview.html', controller: 'OzkerCtrl'});
        $routeProvider.when('/priority', {templateUrl: 'partials/priority.html', controller: 'OzkerCtrl'});
        $routeProvider.otherwise({redirectTo: '/mynetwork'});
  }]).
  config(['$translateProvider', function($translateProvider) {

    $translateProvider.useLoader('$translatePartialLoader', {
      localURLTemplate: "{part}_{lang}.json",
      // We use dynamic JSONP callbacks the way Mother Nature indended
      remoteURLTemplate: g_path.strings+'{part}_{lang}.js?callback=JSON_CALLBACK'
    });
    $translateProvider.preferredLanguage('EN-US');
  }]).
  config(['deviceIconLoaderProvider', function(deviceIconLoaderProvider) {
    deviceIconLoaderProvider.localURL('/images/');
    deviceIconLoaderProvider.remoteURL(g_path.cloud);
    deviceIconLoaderProvider.defaultIcon('UnknownDevice.png');
    deviceIconLoaderProvider.deviceIconTable({
      local: '/trans/devices_icons.json',
      remote: g_path.strings+'devices_icons.js?callback=jsonpDevicesCallback'
    });
  }]).
  config(['flowIconLoaderProvider', function(flowIconLoaderProvider) {
    flowIconLoaderProvider.localURL('/images');
    flowIconLoaderProvider.remoteURL(g_path.cloud);
    flowIconLoaderProvider.defaultIcon('default.png');
  }]);
