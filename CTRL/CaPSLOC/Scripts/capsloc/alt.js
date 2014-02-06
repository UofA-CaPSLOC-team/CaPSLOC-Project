
$(document).ready(function () {

    $('#scan-alts').click(function () {

        var ips = {
            startIP: $('#ip-start').val(),
            endIP: $('#ip-end').val()
        }

        $.ajax({
            url: '/CaPSLOC/ALT/PingSweep',
            type: 'POST',
            data: ips,
            //dataType: 'json',
            //data: JSON.stringify(params),
            //contentType: 'application/json',
            success: function (result) {
                if (result.success) {
                    alert('Ping sweep successful');
                } else {
                    alert('An error occurred during the ping sweep: ' + result.data);
                }
            },
            error: function () {
                alert('An error occurred during the ping sweep');
            }
        });
    });
});