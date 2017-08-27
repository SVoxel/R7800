/*
  Copyright (c) 2013 Qualcomm Atheros, Inc..

  All Rights Reserved.
  Qualcomm Atheros Confidential and Proprietary.
*/

'use strict';

// Declare app level module which depends on filters, and services
angular.module('ozkerDebug', ['ngRoute', 'ozkerDebug.controllers', 'ozkerDebug.filters', 'ozker', 'pascalprecht.translate', 'translateFallbackLoader', 'fallbackLoader', 'iconLoader']).
  config(['$routeProvider', function($routeProvider) {
    $routeProvider.when('/ozker', {templateUrl: 'partials/ozker.html', controller: 'OzkerCtrl'});
    $routeProvider.otherwise({redirectTo: '/ozker'});
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
    deviceIconLoaderProvider.localURL('');
    deviceIconLoaderProvider.remoteURL(g_path.resources);
    deviceIconLoaderProvider.defaultIcon('images/UnknownDevice.png');
    deviceIconLoaderProvider.deviceIconTable({
      local: '/trans/devices_icons.json',
      remote: g_path.strings+'not_used.js'
    });
  }]).

  config(['flowIconLoaderProvider', function(flowIconLoaderProvider) {
    flowIconLoaderProvider.localURL('/images');
    flowIconLoaderProvider.remoteURL(g_path.cloud);
    flowIconLoaderProvider.defaultIcon('default.png');
  }]);
