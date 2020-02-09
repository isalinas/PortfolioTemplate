var vrView;

var scenes = {
  livingRoom: {
    image: "img/livingRoom.jpg",
    hotspots: {
      whaleLeft: {
        pitch: 0,
        yaw: 150,
        radius: 0.05,
        distance: 1
      }
    }
  },
  kitchen: {
    image: "img/kitchen.jpg",
    hotspots: {
      livingRoom: {
        pitch: 0,
        yaw: 110,
        radius: 0.05,
        distance: 1
      }
    }
  }
};

function onLoad() {
  vrView = new VRView.Player("#vrview", {
    image: "img/blank.png",
    preview: "img/blank.png",
    is_stereo: false,
    is_autopan_off: true
  });

  vrView.on("ready", onVRViewReady);
  vrView.on("modechange", onModeChange);
  vrView.on("click", onHotspotClick);
  vrView.on("getposition", onGetPosition);
}

function onVRViewReady(e) {
  console.log("onVRViewReady");
  loadScene("livingRoom");
}

function onModeChange(e) {
  console.log("onModeChange", e.mode);
}

function onGetPosition(e) {
  console.log(e);
}

function onHotspotClick(e) {
  vrView.getPosition();
  console.log("onHotspotClick", e.id);
  if (e.id) {
    loadScene(e.id);
  }
}

function loadScene(id) {
  console.log("loadScene", id);

  // Set the image
  vrView.setContent({
    image: scenes[id].image,
    preview: scenes[id].preview,
    is_stereo: false,
    is_autopan_off: true
  });

  // Add all the hotspots for the scene
  var newScene = scenes[id];
  var sceneHotspots = Object.keys(newScene.hotspots);
  for (var i = 0; i < sceneHotspots.length; i++) {
    var hotspotKey = sceneHotspots[i];
    var hotspot = newScene.hotspots[hotspotKey];

    vrView.addHotspot(hotspotKey, {
      pitch: hotspot.pitch,
      yaw: hotspot.yaw,
      radius: hotspot.radius,
      distance: hotspot.distance
    });
  }
}

(function($) {
  // Remove no-js class
  $("html").removeClass("no-js");

  // Animate to section when nav is clicked
  $("header a").click(function(e) {
    // Treat as normal link if no-scroll class
    if ($(this).hasClass("no-scroll")) return;

    e.preventDefault();
    var heading = $(this).attr("href");
    var scrollDistance = $(heading).offset().top;

    $("html, body").animate(
      {
        scrollTop: scrollDistance + "px"
      },
      Math.abs(window.pageYOffset - $(heading).offset().top) / 1
    );

    // Hide the menu once clicked if mobile
    if ($("header").hasClass("active")) {
      $("header, body").removeClass("active");
    }
  });

  // Scroll to top
  $("#to-top").click(function() {
    $("html, body").animate(
      {
        scrollTop: 0
      },
      500
    );
  });

  // Scroll to first element
  $("#intro-down span").click(function() {
    var scrollDistance = $("#intro")
      .next()
      .offset().top;
    $("html, body").animate(
      {
        scrollTop: scrollDistance + "px"
      },
      500
    );
  });

  // Create timeline
  $("#experience-timeline").each(function() {
    $this = $(this); // Store reference to this
    $userContent = $this.children("div"); // user content

    // Create each timeline block
    $userContent.each(function() {
      $(this)
        .addClass("vtimeline-content")
        .wrap(
          '<div class="vtimeline-point"><div class="vtimeline-block"></div></div>'
        );
    });

    // Add icons to each block
    $this.find(".vtimeline-point").each(function() {
      $(this).prepend(
        '<div class="vtimeline-icon"><i class="fa fa-map-marker"></i></div>'
      );
    });

    // Add dates to the timeline if exists
    $this.find(".vtimeline-content").each(function() {
      var date = $(this).data("date");
      if (date) {
        // Prepend if exists
        $(this)
          .parent()
          .prepend('<span class="vtimeline-date">' + date + "</span>");
      }
    });
  });

  // Open mobile menu
  $("#mobile-menu-open").click(function() {
    $("header, body").addClass("active");
  });

  // Close mobile menu
  $("#mobile-menu-close").click(function() {
    $("header, body").removeClass("active");
  });

  // Load additional projects
  $("#view-more-projects").click(function(e) {
    e.preventDefault();
    $(this).fadeOut(300, function() {
      $("#more-projects").fadeIn(300);
    });
  });
})(jQuery);

window.addEventListener("load", onLoad);
