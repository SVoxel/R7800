/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
'use strict';

////////////////////////////////////////////////////////////////////////
/*
    Class: Filters

    Version:   1.0

    This is the angular filter used for by the flux router
    system. This adds data filters to transform information from the
    angular model used to wrap ozker.

*/
////////////////////////////////////////////////////////////////////////
angular.module('ozkerDebug.filters', [])
    ////////////////////////////////////////////////////////////////////////
    /*
        Function: Mbps

        This defines a custom transform element that is used to
        modify displayed data.

        in your html:
         {{router.model.traffic.up_limit | Mbps}}
        Parameters:
          none.

        Returns:
          nothing.
    */
    ////////////////////////////////////////////////////////////////////////
  .filter('Mbps', function() {
    return function(Bps) {
      return ((Bps * 8)/1000000).toFixed(2);
    }
  });

