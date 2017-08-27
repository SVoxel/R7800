/*
 * Copyright (c) 2014 Qualcomm Atheros, Inc.
 *
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary
 */
/* 3D bar chart d3 reusable component
 * http://bost.ocks.org/mike/chart/
 */

(function() {

d3.tresbar = function() {
  var duration = 0,
      width = 500,
      height = 300,
      x = null,
      y = null,
      bar_colors = d3.scale.category10();

  var base_gradient_colors = {
    'front': ['#718ca1', '#d3eafb'],
    'side': ['#3d4b56', '#83a2ba'],
    'top': ['#94b7d2', '#e3f2fd'],
  };


  function tresbar(bar) {
    new_bar = bar.enter().append("svg")
        .attr("class", "tresbarbar")
        .attr("zIndex", "3")
        .attr("r", "0");

    new_bar.each(function(d, i) {
      var new_bar = d3.select(this);
      var init_height = 0;
      var gradient_front = "gradient-front-" + i;
      var gradient_top = "gradient-top-" + i;
      var gradient_side = "gradient-side-" + i;
      var gcolors = make_gradient_colors(base_gradient_colors, bar_colors(d.tag));

      var defs = new_bar.append("svg:defs");

      var gradient = defs.append("svg:linearGradient")
          .attr("id", gradient_top)
          .attr("gradientUnits", "objectBoundingBox")
          .attr("x1", "0%")
          .attr("y1", "58%")
          .attr("x2", "100%")
          .attr("y2", "42%");
      gradient.append("svg:stop")
          .attr("offset", "0%")
          .attr("stop-color", gcolors['top'][0])
          .attr("stop-opacity", "1");
      gradient.append("svg:stop")
          .attr("offset", "100%")
          .attr("stop-color", gcolors['top'][1])
          .attr("stop-opacity", "1");

      gradient = defs.append("svg:linearGradient")
          .attr("id", gradient_side)
          .attr("gradientUnits", "objectBoundingBox")
          .attr("x1", "55%")
          .attr("y1", "100%")
          .attr("x2", "45%")
          .attr("y2", "0%");
      gradient.append("svg:stop")
          .attr("offset", "0%")
          .attr("stop-color", gcolors['side'][0])
          .attr("stop-opacity", "1");
      gradient.append("svg:stop")
          .attr("offset", "100%")
          .attr("stop-color", gcolors['side'][1])
          .attr("stop-opacity", "1");

      gradient = defs.append("svg:linearGradient")
          .attr("id", gradient_front)
          .attr("gradientUnits", "objectBoundingBox")
          .attr("x1", "50%")
          .attr("y1", "100%")
          .attr("x2", "50%")
          .attr("y2", "0%");
      gradient.append("svg:stop")
          .attr("offset", "0%")
          .attr("stop-color", gcolors['front'][0])
          .attr("stop-opacity", "1");
      gradient.append("svg:stop")
          .attr("offset", "100%")
          .attr("stop-color", gcolors['front'][1])
          .attr("stop-opacity", "1");


      // shadow
      new_bar.append("path")
          .attr("d", function(d) { return make_shadow_path_d(init_height)(d); })
          .attr("class", "tresbarshadow1")
          .attr("fill", "none")
          .attr("isShadow", "true")
          .attr("stroke", "rgb(0,0,0)")
          .attr("stroke-opacity", "0.06")
          .attr("stroke-width", "5")
          .attr("transform", "translate(1,1)");

      new_bar.append("path")
          .attr("d", function(d) { return make_shadow_path_d(init_height)(d); })
          .attr("class", "tresbarshadow2")
          .attr("fill", "none")
          .attr("isShadow", "true")
          .attr("stroke", "rgb(0,0,0)")
          .attr("stroke-opacity", "0.12")
          .attr("stroke-width", "3")
          .attr("transform", "translate(1,1)");

      new_bar.append("path")
          .attr("d", function(d) { return make_shadow_path_d(init_height)(d); })
          .attr("class", "tresbarshadow3")
          .attr("fill", "none")
          .attr("isShadow", "true")
          .attr("stroke", "rgb(0,0,0)")
          .attr("stroke-opacity", "0.18")
          .attr("stroke-width", "1")
          .attr("transform", "translate(1,1)");

      new_bar.append("path")
          .attr("d", function(d) { return make_shadow_path_d(init_height)(d); })
          .attr("class", "tresbarshadow4")
          .attr("fill", "none");

      // front
      new_bar.append("rect")
          .attr("class", "tresbarfront")
          .attr("rx", "0")
          .attr("ry", "0")
          .attr("fill", "url('#" + gradient_front + "')")
          .attr("x", function(d) { return x(d.tag); })
          .attr("y", function(d) { return y(init_height); })
          .attr("width", x.rangeBand())
          .attr("height", function(d) { return Math.max(0, height - y(init_height) - 10); })
          .attr("stroke-width", "0.000001")
          .attr("stroke-opacity", "1")
          .attr("stroke", "#333333");

      // top
      new_bar.append("path")
          .attr("d", function(d) { return make_top_path_d(init_height)(d); })
          .attr("class", "tresbartop")
          .attr("fill", "url('#" + gradient_top + "')")
          .attr("stroke-width", "0.000001")
          .attr("stroke-opacity", "1")
          .attr("stroke", "#333333");

      // side
      new_bar.append("path")
          .attr("d", function(d) { return make_side_path_d(init_height)(d)})
          .attr("class", "tresbarside")
          .attr("fill", "url('#" + gradient_side + "')")
          .attr("stroke-width", "0.000001")
          .attr("stroke-opacity", "1")
          .attr("stroke", "#333333");
    });

    // animate
    bar.selectAll(".tresbarshadow1")
      .transition()
        .duration(duration)
        .attr("d", function(d) { return make_shadow_path_d(d.minutes)(d); })
    bar.selectAll(".tresbarshadow2")
      .transition()
        .duration(duration)
        .attr("d", function(d) { return make_shadow_path_d(d.minutes)(d); })
    bar.selectAll(".tresbarshadow3")
      .transition()
        .duration(duration)
        .attr("d", function(d) { return make_shadow_path_d(d.minutes)(d); })
    bar.selectAll(".tresbarshadow4")
      .transition()
        .duration(duration)
        .attr("d", function(d) { return make_shadow_path_d(d.minutes)(d); })

    bar.selectAll(".tresbarfront")
      .transition()
        .duration(duration)
      .attr("y", function(d) { return y(d.minutes) + 10; })
      .attr("height", function(d) { return Math.max(0, height - y(d.minutes) - 10); })
    bar.selectAll(".tresbartop")
      .transition()
        .duration(duration)
      .attr("d", function(d) { return make_top_path_d(d.minutes)(d); })
    bar.selectAll(".tresbarside")
      .transition()
        .duration(duration)
      .attr("d", function(d) { return make_side_path_d(d.minutes)(d); })
  }

  tresbar.duration = function(x) {
    if (!arguments.length) return duration;
    duration = x;
    return tresbar;
  };

  tresbar.width = function(x) {
    if (!arguments.length) return width;
    width = x;
    return tresbar;
  };

  tresbar.height = function(x) {
    if (!arguments.length) return height;
    height = x;
    return tresbar;
  };

  tresbar.colors = function(x) {
    if (!arguments.length) return bar_colors;
    bar_colors = x;
    return tresbar;
  };

  tresbar.x = function(foo) {
    if (!arguments.length) return x;
    x = foo;
    return tresbar;
  };

  tresbar.y = function(foo) {
    if (!arguments.length) return y;
    y = foo;
    return tresbar;
  };

  function make_shadow_path_d(yval) {
    function shadow_path_d(d) {
      var width = x.rangeBand();
      var x1 = x(d.tag) + 10;
      var x2 = x1 + width;
      var y1 = Math.min(height - 10, y(yval));
      var y2 = height - 10;
      return "M " + x1 + " " + y1 + " L " + x2 + " " + y1 + " " + x2 + " " + y2;
    };
    return shadow_path_d;
  };

  function make_top_path_d(yval) {
    function top_path_d(d) {
      var width = x.rangeBand();
      var x1 = x(d.tag);
      var x2 = x1 + width;
      var x3 = x2 + 10;
      var x4 = x1 + 10;
      var y1 = Math.min(height - 10, y(yval));
      var y2 = y1 + 10;
      return "M " + x1 + " " + y2 + " L " + x2 + " " + y2 + " " + x3 + " " + y1 + " " + x4 + " " + y1 + " Z";
    };
    return top_path_d;
  };

  function make_side_path_d(yval) {
    function side_path_d(d) {
      var width = x.rangeBand();
      var x1 = x(d.tag) + width;
      var x2 = x1 + 10;
      var y1 = Math.min(height - 10, y(yval));
      var y2 = y1 + 10;
      var y3 = height - 10;
      return "M " + x1 + " " + y2 + " L " + x2 + " " + y1 + " " + x2 + " " + y3 + " " + x1 + " " + height + " Z";
    };
    return side_path_d
  };

  /* Use the hue from the color and return the set of gradient colors */
  function make_gradient_colors(base_gradient_colors, color) {
    var hue = d3.hsl(color).h;
    var gcolors = {};
    for (var face in base_gradient_colors) {
      gcolors[face] = [];
      var base_face_colors = base_gradient_colors[face];
      for (var i = 0; i < base_face_colors.length; i++) {
        var base_face_color = d3.hsl(base_face_colors[i]);
        face_color = d3.hsl(hue, base_face_color.s, base_face_color.l);
        gcolors[face].push(face_color.toString());
      }
    }
    return gcolors;
  }

  return tresbar;
};


})();

