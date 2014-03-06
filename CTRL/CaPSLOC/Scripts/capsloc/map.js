﻿
$(document).ready(function () {

    google.maps.event.addDomListener(window, 'load', initializeMap);
    $('#location-submit').click(submitLocation);
    initializeMap();

    // Remove the unobtrusive validator so we can manually set up validation
    $('#map-page-form').removeData('validator');
    $('#map-page-form').validate({
        rules: {
            'latitude-field': {
                required: true,
                number: true,
                range: [-90, 90]
            },
            'longitude-field': {
                required: true,
                number: true,
                range: [-180, 180]
            },
            'altitude-field': {
                required: true,
                number: true
            },
            'name-field': {
                required: true
            }
        },
        messages: {
            'latitude-field': {
                required: 'Latitude is required',
                number: 'Latitude must be a number',
                range: 'Latitude must be between -90 and 90'
            },
            'longitude-field': {
                required: 'Longitude is required',
                number: 'Longitude must be a number',
                range: 'Longitude must be between -180 and 180'
            },
            'altitude-field': {
                required: 'Altitude is required',
                number: 'Altitude must be a number'
            },
            'name-field': {
                required: 'Name is required'
            }
        },
        errorContainer: '#map-page-error',
        errorLabelContainer: '#map-page-error ul',
        wrapper: 'li'
    });
});

var map;
var marker;

function initializeMap() {
    var mapOptions = {
        zoom: 4,
        minZoom:4,
        center: new google.maps.LatLng(-25.363882, 131.044922),
        mapTypeId: google.maps.MapTypeId.ROADMAP
    };

    $('#map-canvas').empty();  // Clear this out, so we don't have multiple maps
    map = new google.maps.Map(document.getElementById('map-canvas'),
      mapOptions);

    marker = new google.maps.Marker({
        map: map,
        position: new google.maps.LatLng(37.7699298, -122.4469157),
        title: 'Click on map to move'
    });

    google.maps.event.addListener(map, 'click', function (event) {
        addMarker(event.latLng);
    });
}

// Add a marker to the map and push to the array.
function addMarker(location) {
    marker.setMap(null);
    marker = new google.maps.Marker({
        position: location,
        map: map
    });
    var latlng = marker.getPosition();
    $('#latitude-field').val(latlng.lat());
    $('#longitude-field').val(latlng.lng());

    var request = {
        locations: [latlng]
    }
    var eServ = new google.maps.ElevationService();
    eServ.getElevationForLocations(request, setAltitude);
}

function setAltitude(locations, status){
    $('#altitude-field').val(locations[0].elevation);
}

function submitLocation() {

    if ($('#map-page-form').valid()) {
        var location = {
            Latitude: $('#latitude-field').val(),
            Longitude: $('#longitude-field').val(),
            Altitude: $('#altitude-field').val(),
            Name: $('#name-field').val()
        };

        $.ajax({
            url: '/CaPSLOC/Map/SaveLocation',
            type: 'POST',
            data: location
        });
    }
}