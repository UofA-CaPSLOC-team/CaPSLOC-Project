
var _scriptContext = [];

$(document).ready(function () {
    $('#command-list').change(function () {
        $('.command-parameters').css('display', 'none');
        var selectVal = $('#command-list').val();
        $('#command-parameters-' + selectVal).css('display', 'block');

        if (selectVal == 2) {
            loadLocations();
        }
    });

    $('#script-display').flexigrid({
        colModel: [
            { display: 'Command', name: 'command', width: 150, sortable: false, align: 'left' },
            { display: 'Parameter Name', name: 'param_name', width: 150, sortable: false, align: 'left' },
            { display: 'Parameter Value', name: 'param_value', width: 150, sortable: false, align: 'left' },
        ],
        buttons: [
        ],
        title: 'SCRIPT_NAME_HERE',
        width: 450,
        height: 300,
        singleSelect: true
    });

    $('#add-script-element').button().click(function () {
        var data = readScriptInput($('#command-list').val());

        $.ajax({
            url: '/CaPSLOC/Map/Garbage',
            type: 'POST',
            data: JSON.stringify(data),
            dataType: 'json',
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

function readScriptInput(cmdType) {
    var serializedInput = { commandType: cmdType, params: [] };
    serializedInput.commandId = Math.floor(Math.random() * 0xFFFFFFFF).toString(16);

    switch (parseInt(cmdType)) {
        case 1:
            serializedInput.commandName = 'Go To Location';
            serializedInput.params.push({
                id: serializedInput.commandId + '-1',
                name: 'Latitude',
                value: $('#latitude-script-field').val()
            });
            serializedInput.params.push({
                id: serializedInput.commandId + '-2',
                name: 'Longitude',
                value: $('#longitude-script-field').val()
            });
            serializedInput.params.push({
                id: serializedInput.commandId + '-3',
                name: 'Altitude',
                value: $('#altitude-script-field').val()
            });
            break;
        case 2:
            serializedInput.commandName = 'Go To Location';
            alert("Not Implemented!");  // Need to read back that value from the server (or store when pulling back all locations)
            /*serializedInput.params.push({
            id: serializedInput.commandId + '-1',
            name: 'Latitude',
            value: $('#latitude-script-field').val()
            });
            serializedInput.params.push({
            id: serializedInput.commandId + '-2',
            name: 'Longitude',
            value: $('#longitude-script-field').val()
            });
            serializedInput.params.push({
            id: serializedInput.commandId + '-3',
            name: 'Altitude',
            value: $('#altitude-script-field').val()
            });
            serializedInput.params.push({
            id: serializedInput.commandId + '-4',
            name: 'Name',
            value: $('#altitude-script-field').val()
            });*/
            break;
        case 3:
            serializedInput.commandName = 'Halt';
            break;
        case 4:
            serializedInput.commandName = 'Pause';
            break;
        case 5:
            serializedInput.commandName = 'Resume';
            break;
        case 6:
            serializedInput.commandName = 'Image Capture';
            serializedInput.params.push({
                id: serializedInput.commandId + '-1',
                name: 'Image Type',
                value: $('input[name=image-capture-type]:checked').val()
            });
            if ($('#image-capture-duration').val() && $('input[name=image-capture-type]:checked').val() == 2) {
                serializedInput.params.push({
                    id: serializedInput.commandId + '-2',
                    name: 'Duration',
                    value: $('#image-capture-duration').val()
                });
            }
            if ($('#image-capture-frame-rate').val() && $('input[name=image-capture-type]:checked').val() == 2) {
                serializedInput.params.push({
                    id: serializedInput.commandId + '-3',
                    name: 'Frame Rate',
                    value: $('image-capture-frame-rate').val()
                });
            }
            if ($('#image-capture-resolution').val()) {
                serializedInput.params.push({
                    id: serializedInput.commandId + '-4',
                    name: 'Resolution',
                    value: $('#image-capture-resolution').val()
                });
            }
            break;
        case 7:
            serializedInput.commandName = 'Rotate';
            serializedInput.params.push({
                id: serializedInput.commandId + '-1',
                name: 'Direction',
                value: $('#rotate-direction').val()
            });
            if ($('#rotate-degrees').val()) {
                serializedInput.params.push({
                    id: serializedInput.commandId + '-2',
                    name: 'Degrees',
                    value: $('#rotate-degrees').val()
                });
            }
            break;
        case 8:
            serializedInput.commandName = 'Wait';
            if ($('#wait-duration').val()) {
                serializedInput.params.push({
                    id: serializedInput.commandId + '-1',
                    name: 'Duration',
                    value: $('#wait-duration').val()
                });
            }
            break;
        case 9:
            serializedInput.commandName = 'Execute Script';
            serializedInput.params.push({
                id: serializedInput.commandId + '-1',
                name: 'Script Name',
                value: $('#execute-name').val()
            });
            break;
        case 10:
            serializedInput.commandName = 'Set Defaults';
            if ($('#default-rotate-degrees').val()) {
                serializedInput.params.push({
                    id: serializedInput.commandId + '-1',
                    name: 'Degrees',
                    value: $('#default-rotate-degrees').val()
                });
            }
            if ($('#default-image-capture-duration').val()) {
                serializedInput.params.push({
                    id: serializedInput.commandId + '-2',
                    name: 'Video Duration',
                    value: $('#default-image-capture-duration').val()
                });
            }
            if ($('#default-image-capture-frame-rate').val()) {
                serializedInput.params.push({
                    id: serializedInput.commandId + '-3',
                    name: 'Video Frame Rate',
                    value: $('default-image-capture-frame-rate').val()
                });
            }
            if ($('#default-image-capture-resolution').val()) {
                serializedInput.params.push({
                    id: serializedInput.commandId + '-4',
                    name: 'Image Resolution',
                    value: $('#default-image-capture-resolution').val()
                });
            }
            if ($('#default-wait-duration').val()) {
                serializedInput.params.push({
                    id: serializedInput.commandId + '-5',
                    name: 'Wait Duration',
                    value: $('#default-wait-duration').val()
                });
            }
            if ($('#latitude-offset').val()) {
                serializedInput.params.push({
                    id: serializedInput.commandId + '-6',
                    name: 'Latitude Offset',
                    value: $('#latitude-offset').val()
                });
            }
            if ($('#longitude-offset').val()) {
                serializedInput.params.push({
                    id: serializedInput.commandId + '-7',
                    name: 'Longitude Offset',
                    value: $('#longitude-offset').val()
                });
            }
            if ($('#altitude-offset').val()) {
                serializedInput.params.push({
                    id: serializedInput.commandId + '-8',
                    name: 'Altitude Offset',
                    value: $('#altitude-offset').val()
                });
            }
            break;
        default:
            alert('Invalid command type');
            break;
    }
    return serializedInput;
}