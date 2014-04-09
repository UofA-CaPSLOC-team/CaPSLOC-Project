
$(document).ready(function () {
    imageRefreshALTs();
    imageRefreshLocations();

    $('#image-filter-refresh').button({ icons: { primary: 'ui-icon-arrowrefresh-1-s'} }).click(function () {
        if ($('#image-filter-form').valid()) {
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
                dataType: 'html',
                data: JSON.stringify(filterData),
                contentType: 'application/json',
                success: function (result) {
                    $('#image-gallery-grid').html(result);
                },
                error: function () {
                    alert('An error occurred while finding the images');
                }
            });
        }
    });

    // Remove the unobtrusive validator so we can manually set up validation
    $('#image-filter-form').removeData('validator');
    $('#image-filter-form').validate({
        rules: {
            'image-start-date': {
                required: {
                    depends: function (element) {
                        return $('#image-start-time').val();
                    }
                }
            },
            'image-start-time': {
                required: {
                    depends: function (element) {
                        return $('#image-start-date').val();
                    }
                }
            },
            'image-end-date': {
                required: {
                    depends: function (element) {
                        return $('#image-end-time').val();
                    }
                }
            },
            'image-end-time': {
                required: {
                    depends: function (element) {
                        return $('#image-end-date').val();
                    }
                }
            }
        },
        messages: {
            'image-start-date': {
                required: 'Start date is required if a start time is given.'
            },
            'image-start-time': {
                required: 'Start time is required if a start date is given.'
            },
            'image-end-date': {
                required: 'End date is required if an end time is given.'
            },
            'image-end-time': {
                required: 'End time is required if an end date is given.'
            }
        },
        errorContainer: '#image-filter-error',
        errorLabelContainer: '#image-filter-error ul',
        wrapper: 'li'
    });
});

function imageRefreshALTs() {
    var $altList = $('#image-alt-list');
    $altList.empty();
    $('<option/>').val('0').text('<All ALTs>').appendTo($altList);
    $.ajax({
        url: '/CaPSLOC/ALT/All',
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