
$(document).ready(function () {

    var tabs = $('#tabs');
    tabs.tabs();

    google.maps.event.addDomListener(window, 'load', initialize);
    initialize();
});

var map;
var marker;

function initialize() {
    var mapOptions = {
        zoom: 4,
        minZoom:4,
        center: new google.maps.LatLng(-25.363882, 131.044922),
        mapTypeId: google.maps.MapTypeId.ROADMAP
    };

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