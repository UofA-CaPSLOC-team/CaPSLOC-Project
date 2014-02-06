
$(document).ready(function () {

    $('#interactive-command-list').change(function () {
        $('.command-parameters').css('display', 'none');
        var selectVal = $('#interactive-command-list').val();
        $('#interactive-command-parameters-' + selectVal).css('display', 'block');

        if (selectVal == 2) {
            loadLocations();
        }
    });

    $('#send-command').button().click(function () {
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
    });
});