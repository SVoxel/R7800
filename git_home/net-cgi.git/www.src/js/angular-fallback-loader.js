/*
  Copyright (c) 2013 Qualcomm Atheros, Inc..

  All Rights Reserved.
  Qualcomm Atheros Confidential and Proprietary.
*/

'use strict';

angular.module('fallbackLoader', []).

service("loader", ['$q', '$http', function($q, $http) {

  function loadJSON(localURL, remoteURL) {
    var localPromise, remotePromise;

    //if we have a url
    if(localURL!="")
    {
      localPromise = $http({
        method : 'GET',
        url : localURL
      });
    }
    else // else we don't have a url return an unfulfillable promise
    {
      localPromise = $q.defer().promise;
    }

   /* old way...we can do this again if we get server side 
      web capabliity that can generate on the fly json_callback handlers

      cloud front doesn't do that.

    remotePromise = $http({
      url: remoteURL,
      method: 'JSONP',
      params: ''
    });*/
    remotePromise = getJSONP(remoteURL);

    return {local: localPromise, remote: remotePromise};
  };

  function getJSONP(remoteURL)
  {
    //first set up our promise of return
    var deferred = $q.defer();

    //get the calback param
    var name = "callback";
    var results = new RegExp('[\\?&]' + name + '=([^&#]*)').exec(remoteURL);

    //if we have a calback
    if(results !=undefined && results.length>=1)
    {
          //find the 1st param
          var nEnd = remoteURL.indexOf('?');

          //if we dont have a callback
          if(nEnd == -1)
          {
            nEnd = remoteURL.length;
          }

          //strip the callback from the string
          var strurl = remoteURL.substring(0,nEnd);

          //do our ajax call old style
          $.ajax(
                  {
                      type: "GET",
                      async: true,
                      url: strurl,
                      jsonpCallback:  results[1],
                      contentType: "application/json",
                      dataType: 'jsonp',
                      success: function(data, status, request) // we got the jsonp
                      {
                         var r = remoteURL;
                         var result = { 'data': null };

                         result.data = data;

                         if(typeof data.flows != 'undefined')
                         {
                           //return the success
                           result.data = data.flows;
                         }
                         else if(typeof data.trans != 'undefined')
                         {
                           //return the success
                          result.data = data.trans;
                         }
                         else if(typeof data.devices != 'undefined')
                         {
                           //return the success
                          result.data = data.devices;
                         }

                         deferred.resolve(result);
                      },
                      error: function(data, status, request) // we didnt get jsonp
                      {
                        //return the error
                        deferred.reject("error");
                      }

          });
    }
    else // if we are here no jsonp callback was defined
    {
       deferred.reject("No callback=callback in URL");
    }

    //return the delayed call
    return deferred.promise;
  }

  function loadImage(localURL, remoteURL) {
    var localDeferred,
        remoteDeferred,
        localImage,
        remoteImage;

    localDeferred = $q.defer();
    remoteDeferred = $q.defer();

    //console.log("start loading local image");
    localImage = new Image();
    localImage.onload = function() {
      localDeferred.resolve(localURL);
    };
    localImage.onerror = function() {
      localDeferred.reject("error");
    };
    localImage.onabort = function () {
      localDeferred.reject("abort");
    }
    localImage.src = localURL;

    //console.log("start loading remote image");
    remoteImage = new Image();
    remoteImage.onload = function() {
      remoteDeferred.resolve(remoteURL);
    };
    remoteImage.onerror = function() {
      remoteDeferred.reject("error");
    };
    remoteImage.onabort = function () {
      remoteDeferred.reject("abort");
    }
    remoteImage.src = remoteURL;

    return {local: localDeferred.promise, remote: remoteDeferred.promise};
  };

  function fallback(promises) {
    var deferred,
        localResult,
        remoteResult,
        haveLocal,
        haveRemote,
        localHasFailed,
        remoteHasFailed;

    deferred = $q.defer();
    haveLocal = false;
    haveRemote = false;
    localHasFailed = false;
    remoteHasFailed = false;

    promises.local.then(
      function(result) {
        haveLocal = true;
        localResult = result;
        if (remoteHasFailed) {
          deferred.resolve(result);
        } else if (! haveRemote) {
          deferred.notify(result);
        }
      },
      function(reason) {
        localHasFailed = true;
        if (remoteHasFailed) {
          deferred.reject("Both local and remote have failed");
        }
      }
    );

    promises.remote.then(
      function(result) {
        haveRemote = true;
        remoteResult = result;
        deferred.resolve(result);
      },
      function(reason) {
        remoteHasFailed = true;
        if (haveLocal) {
          deferred.resolve(localResult)
        }
        else if (localHasFailed) {
          deferred.reject("Both local and remote have failed");
        }
      }
    );

    return deferred.promise;
  };

  return {
    loadJSON: loadJSON,
    loadImage: loadImage,
    fallback: fallback,
  };
}]);
