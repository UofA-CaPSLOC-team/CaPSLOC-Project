
$(document).ready(function () {
    $('#command-list').change(function () {
        $('.command-parameters').css('display', 'none');
        var selectVal = $('#command-list').val();
        $('#command-parameters-' + selectVal).css('display', 'block');

        if (selectVal == 2) {
            loadLocations();
        }
    });
});

function loadLocations() {

    $('#goto-location-list').empty();  // Remove existing locations

    $.ajax({
        url: '/CaPSLOC/Map/AllLocations',
        type: 'GET',
        success: function (result) {
            if (result.success) {
                $.each(result.data, function (index, element) {
                    $('<option/>').val(element.Id).text(element.Name).appendTo('#goto-location-list');
                });
            }
            else {
                alert('Error returned from server');
            }
        },
        error: function () {
            alert('Failed to load locations');
        }
    });
}