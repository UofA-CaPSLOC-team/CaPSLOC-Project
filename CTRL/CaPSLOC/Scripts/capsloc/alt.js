
$(document).ready(function () {

    // This isn't technically correct, but will do in a pinch
    $.validator.addMethod("IPv4Address", function (value, element) {
        var ipv4 = /^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$/;
        return this.optional(element) || value.match(ipv4);
    }, "Invalid IP address");

    $.validator.addMethod("SameSubnet", function (value, element, param) {
        var paramVal = param.val();
        var sub1 = value.substr(0, value.lastIndexOf('.'));
        var sub2 = paramVal.substr(0, paramVal.lastIndexOf('.'));
        return value == "" || sub1 == sub2;  // If this is blank, we will only use the starting IP
    }, "IP addresses are not in same subnet");

    $.validator.addMethod("HigherIP", function (value, element, param) {
        if (value == "") return true; // If this is blank, we will only use the starting IP
        var paramVal = param.val();
        var ipString1 = paramVal.split('.');
        var ipInt1 = parseInt(ipString1[0]) << 24 | parseInt(ipString1[1]) << 16 | parseInt(ipString1[2]) << 8 | parseInt(ipString1[3])
        var ipString2 = value.split('.');
        var ipInt2 = parseInt(ipString2[0]) << 24 | parseInt(ipString2[1]) << 16 | parseInt(ipString2[2]) << 8 | parseInt(ipString2[3])
        return ipInt1 <= ipInt2;
    }, "End of IP range is lower than beginning");

    var locateAltGrid = $('#alts-located');
    locateAltGrid.flexigrid({
        colModel: [
            { display: 'ALT Name', name: 'command', width: 150, sortable: false, align: 'left' },
            { display: 'IP Address', name: 'param_name', width: 150, sortable: false, align: 'left' }
        ],
        dataType: 'json',
        buttons: [{
            name: 'Reboot',
            onpress: function () {
                var selectedRows = $('#alts-located .trSelected');
                if (selectedRows.length > 0) {
                    var altId = selectedRows.attr('id').substr(3);
                    $.ajax({
                        url: '/CaPSLOC/ALT/Reboot',
                        type: 'GET',
                        data: { 'altId': altId },
                        success: function (result) {
                            alert(result.data);
                        },
                        error: function () {
                            alert('An error occurred while sending the reboot command');
                        }
                    });
                } else {
                    alert('Please select a row!');
                }
            },
            bclass: 'add'
        }],
        title: 'Available ALT Units',
        width: 450,
        height: 300,
        singleSelect: true
    });

    // Remove the unobtrusive validator so we can manually set up validation
    $('#alt-locate-form').removeData('validator');
    $('#alt-locate-form').validate({
        rules: {
            'ip-start': {
                IPv4Address: true,
                required: true
            },
            'ip-end': {
                IPv4Address: true,
                SameSubnet: $('#ip-start'),
                HigherIP: $('#ip-start')
            }
        },
        errorContainer: '#alt-locate-error',
        errorLabelContainer: '#alt-locate-error ul',
        wrapper: 'li'
    });

    $('#scan-alts').click(function () {
        if ($('#alt-locate-form').valid()) {
            var ips = {
                startIP: $('#ip-start').val(),
                endIP: $('#ip-end').val() != '' ? $('#ip-end').val() : $('#ip-start').val()  // Scan single IP if only one is defined
            }

            openWaitDialog();
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
                    closeWaitDialog();
                },
                error: function () {
                    alert('An error occurred during the ping sweep');
                    closeWaitDialog();
                }
            });
        }
    });
});