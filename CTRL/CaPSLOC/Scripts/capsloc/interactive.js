// refresh the list of debug messages
function debugRefresh() {
    var useAll = $('input[name=interactive-debug-type]:checked').val();
    if(useAll != 0){
        useAll = $('#interactive-alt-list').val();  // Figure out what the active ALT is
    }
    if(!useAll){
        useAll = 0;  // First time through it may not have loaded yet
    }

  $.ajax({
    url: '/CaPSLOC/ALT/Debug?AltId=' + useAll,
    type: 'GET', 
    success: function(result) {
        if (result.success) {
            $('#interactive-debug').empty();
            $.each(result.data, function(index, element){
                var debugString = new Date(parseInt(element.Time.substr(6))).toLocaleString() + ' - ' + element.AltName + ': ' + element.Message;
                $('<p/>').text(debugString).appendTo($('#interactive-debug'));
            });
        }
    },
    complete: function() {
      // Schedule the next request when the current one's complete
      _debugRefreshId = setTimeout(debugRefresh, 5000);
    }
  });
}

$(document).ready(function () {

    $('#interactive-command-list').change(function () {
        $('.command-parameters').css('display', 'none');
        var selectVal = $('#interactive-command-list').val();
        $('#interactive-command-parameters-' + selectVal).css('display', 'block');
    });

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

    // When the radio button changes, cancel the pending reload and fire immediately
    $('input[name=interactive-debug-type]').change(function(){    
        clearTimeout(_debugRefreshId);
        debugRefresh();
    });
    $('#interactive-alt-list').change(function(){    
        clearTimeout(_debugRefreshId);
        debugRefresh();
        interactiveRefreshScriptList();
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

var altIdForLoad;

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
                $('#interactive-alt-list').change();  // This is like a change, but the event doesn't get fired
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
                alert('An error occurred while finding the locations: ' + result.data);
            }
        },
        error: function () {
            alert('An error occurred while finding the locations');
        }
    });
}

function interactiveRefreshScriptList() {

    var altId = $('#interactive-alt-list').val();

    if(altId != null){
        $.ajax({
            url: '/CaPSLOC/ALT/Scripts?altId=' + altId,
            type: 'GET',
            success: function (result) {
                if (result.success) {
                    var $list = $('#interactive-script-list');
                     $list.empty();

                    $.each(result.data, function (index, element) {
                        $('<option/>').val(element.Name).text(element.Name).appendTo($list);        
                    });
                } else {
                    alert('An error occurred while refreshing the script list: ' + result.data);
                }
            },
            error: function () {
                alert('An error occurred while refreshing the script list');
            }
         });
     }
}