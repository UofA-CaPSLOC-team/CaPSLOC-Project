

$(document).ready(function () {

    $('#interactive-command-list').change(function () {
        $('.command-parameters').css('display', 'none');
        var selectVal = $('#interactive-command-list').val();
        $('#interactive-command-parameters-' + selectVal).css('display', 'block');

        if (selectVal == 2) {
            loadLocations();
        }
    });

    interactiveRefreshLocations();
    interactiveRefreshALTs();

    $('#send-command').button().click(function () {
        if($('#interactive-command-form').valid()){
            var com = readScriptInput($('#interactive-command-list').val(), $('#interactive-mode'));
            var alt = $('#interactive-alt-list').val();

            $.ajax({
                url: '/CaPSLOC/ALT/SendCommand',
                type: 'POST',
                dataType: 'json',
                data: JSON.stringify({AltId: alt, UserLevel: 1, Command: com}),
                contentType: 'application/json',
                success: function (result) {
                    if (result.success) {
                        alert('Command sent successfully');
                    } else {
                        alert('An error occurred while sending the command: ' + result.data);
                    }
                },
                error: function () {
                    alert('An error occurred while sending the command');
                }
            });
        }
    });

    // Remove the unobtrusive validator so we can manually set up validation
    $('#interactive-command-form').removeData('validator');
    $('#interactive-command-form').validate({
        rules: {
            'latitude-script-field': {
                required: true,
                number: true,
                range: [-90, 90]
            },
            'longitude-script-field': {
                required: true,
                number: true,
                range: [-180, 180]
            },
            'altitude-script-field': {
                required: true,
                number: true
            },
            'execute-name': {
                required: true
            },
        },
        errorContainer: '#interactive-command-error',
        errorLabelContainer: '#interactive-command-error ul',
        wrapper: 'li'
    });
});

function interactiveRefreshALTs(){
    var $altList = $('#interactive-alt-list');
    $altList.empty();
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

function interactiveRefreshLocations(){
    var $locs = $('#interactive-goto-location-list');
    $locs.empty();
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
                alert('An error occurred while finding the ALTs: ' + result.data);
            }
        },
        error: function () {
            alert('An error occurred while finding the ALTs');
        }
    });
}