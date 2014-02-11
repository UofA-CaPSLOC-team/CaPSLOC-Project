
$(document).ready(function () {

// This isn't technically correct, but will do in a pinch
    $.validator.addMethod("IPv4Address", function (value, element) {
        var ipv4 = /^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$/;
        return this.optional(element) || value.match(ipv4);
    }, "Invalid IP address");

    var locateAltGrid = $('#alts-located');
    locateAltGrid.flexigrid({
        colModel: [
            { display: 'ALT Name', name: 'command', width: 150, sortable: false, align: 'left' },
            { display: 'IP Address', name: 'param_name', width: 150, sortable: false, align: 'left' }
        ],
        dataType: 'json',
        title: 'Available ALT Units',
        width: 450,
        height: 300,
        singleSelect: true
    });

    // Remove the unobtrusive validator so we can manually set up validation
    $('#alt-locate-form').removeData('validator');
    $('#alt-locate-form').validate({
        rules: {
            'ip-start': 'IPv4Address',
            'ip-end': 'IPv4Address'
        },
        errorContainer: '#alt-locate-error',
        errorLabelContainer: '#alt-locate-error ul',
        wrapper: 'li'
    });

    $('#scan-alts').click(function () {
        if ($('#alt-locate-form').valid()) {
            var ips = {
                startIP: $('#ip-start').val(),
                endIP: $('#ip-end').val()
            }

            $.ajax({
                url: '/CaPSLOC/ALT/PingSweep',
                type: 'POST',
                data: ips,  // No JSON
                success: function (result) {
                    if (result.success) {
                        var newData = { page: 1, total: result.data.length, rows: [] };
                        $.each(result.data, function (index, element) {
                            newData.rows.push({ id: element.Id, cell: [element.Name, element.Address] });
                        });
                        $('#alts-located').flexAddData(newData);
                    } else {
                        alert('An error occurred during the ping sweep: ' + result.data);
                    }
                },
                error: function () {
                    alert('An error occurred during the ping sweep');
                }
            });
        }
    });
});