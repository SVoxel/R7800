/*
  Copyright (c) 2013 Qualcomm Atheros, Inc..

  All Rights Reserved.
  Qualcomm Atheros Confidential and Proprietary.
*/

'use strict';

angular.module('iconLoader', ['fallbackLoader']).

provider('deviceIconLoader', function() {
  var remoteURL,
      localURL,
      defaultIcon,
      defaultURL,
      deviceIconTableLocalURL,
      deviceIconTableRemoteURL;

  this.remoteURL = function(url) {
    remoteURL = url;
  };

  this.localURL = function(url) {
    localURL = url;
  };

  this.defaultIcon = function(icon) {
    defaultIcon = icon;
  };

  this.deviceIconTable = function(urls) {
    deviceIconTableLocalURL = urls.local;
    deviceIconTableRemoteURL = urls.remote;
  };


  this.$get = ['loader', function(loader) {
    var icons = {};
    var deviceToIcon = {};
    var tableLoader;

    defaultURL = URI(localURL + "/" + defaultIcon).normalizePathname().toString();
    // Load device to icon table - don't send a local because we always want this to come from the cloudses
    tableLoader = loader.fallback(loader.loadJSON("", deviceIconTableRemoteURL)).then(
      function(result) {
        deviceToIcon = result.data;
      },
      null,
      function(result) {
        deviceToIcon = result.data;
      }
    );

    function load(name) {
      var url,
          localIconURL,
          remoteIconURL,
          iconName;

      if (icons.hasOwnProperty(name)) {
        url = icons[name];
      } else {
        url = defaultURL;
        icons[name] = url;
        tableLoader.then(function() {
          iconName = deviceToIcon[name];

          localIconURL = URI(localURL + iconName).normalizePathname().toString();
          remoteIconURL = URI(remoteURL + '/' + iconName).normalizePathname().toString();
          loader.fallback(loader.loadImage(localIconURL, remoteIconURL)).then(
            function(url) {
              icons[name] = url;
            },
            null,
            function(url) {
              icons[name] = url;
            }
          );
        });
      }
      return url;
    };

    return {
      icons: icons,
      load: load,
    };
  }];

}).

provider('flowIconLoader', function() {
  var remoteURL,
      localURL,
      defaultIcon,
      defaultURL;

  this.remoteURL = function(url) {
    remoteURL = url;
  };

  this.localURL = function(url) {
    localURL = url;
  };

  this.defaultIcon = function(icon) {
    defaultIcon = icon;
  };

  this.$get = ['loader', function(loader) {
    var icons = {};
    
    defaultURL = URI(localURL + "/" + defaultIcon).normalizePathname().toString();
    function load(name) {
      var url,
          localIconURL,
          remoteIconURL,
          iconName;

      if (icons.hasOwnProperty(name)) {
        url = icons[name];
      } else {
        url = defaultURL;
        icons[name] = url;

        localIconURL = URI(localURL + '/' + name + '.png').normalizePathname().toString();
        remoteIconURL = URI(remoteURL + '/' + name + '.png').normalizePathname().toString();
        loader.fallback(loader.loadImage(localIconURL, remoteIconURL)).then(
          function(url) {
            icons[name] = url;
          },
          null,
          function(url) {
            icons[name] = url;
          }
        );
      }
      return url;
    };

    return {
      icons: icons,
      load: load,
    };
  }];

});
