
$(document).ready(function () {
    imageRefreshALTs();
    imageRefreshLocations();

    $('#image-filter-refresh').button({ icons: { primary: 'ui-icon-arrowrefresh-1-s'} }).click(function () {

        var filterData = {
            AltId: $('#image-alt-list').val(),
            LocationId: $('#image-location-list').val(),
            StartDate: $('#image-start-date').val(),
            StartTime: $('#image-start-time').val(),
            EndDate: $('#image-end-date').val(),
            EndTime: $('#image-end-time').val()
        };

        $.ajax({
            url: '/CaPSLOC/Image/RefreshGrid',
            type: 'POST',
            dataType: 'json',
            data: JSON.stringify(filterData),
            contentType: 'application/json',
            success: function (result) {
                alert('Returned successfully');
            },
            error: function () {
                alert('An error occurred while finding the images 2');
            }
        });
    });
});

function imageRefreshALTs() {
    var $altList = $('#image-alt-list');
    $altList.empty();
    $('<option/>').val('0').text('<All ALTs>').appendTo($altList);
    $.ajax({
        url: '/CaPSLOC/ALT/RecentlyLocated',
        type: 'GET',
        success: function (result) {
            if (result.success) {
                $.each(result.data, function (index, element) {
                    $('<option/>').val(element.Id).text(element.Name + ' (' + element.Address + ')').appendTo($altList);
                });
            } else {
                alert('An error occurred while finding the ALTs: ' + result.data);
            }
        },
        error: function () {
            alert('An error occurred while finding the ALTs');
        }
    });
}

function imageRefreshLocations() {
    var $locs = $('#image-location-list');
    $locs.empty();
    $('<option/>').val('0').text('<All Locations>').appendTo($locs);
    $.ajax({
        url: '/CaPSLOC/Map/AllLocations',
        type: 'GET',
        success: function (result) {
            if (result.success) {
                $.each(result.data, function (index, element) {
                    $('<option/>')
                    .val(element.Id)
                    .text(element.Name + ' (' + element.Latitude + ', ' + element.Longitude + ', ' + element.Altitude + ')')
                    .appendTo($locs);
                });
            } else {
                alert('An error occurred while finding the locations: ' + result.data);
            }
        },
        error: function () {
            alert('An error occurred while finding the locations');
        }
    });
}