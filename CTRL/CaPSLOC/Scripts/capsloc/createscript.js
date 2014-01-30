
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
        dataType: 'json',
        buttons: [{
            name: 'Delete',
            onpress: deleteScriptElement
        }],
        title: 'SCRIPT_NAME_HERE',
        width: 450,
        height: 300,
        singleSelect: true
    });

    $('#save-script-dialog').dialog({
        autoOpen: false,
        height: 500,
        width: 350,
        title: 'Save Script',
        // modal: true,
        buttons: {
            Save: saveScript,
            Cancel: function () {
                $(this).dialog('close');
            }
        }
    });


    $('#add-script-element').button().click(function () {
        var data = readScriptInput($('#command-list').val());

        _scriptContext.push(data);

        reloadScriptGrid();
    });

    $('#save-script').button().click(function () {
        $('#save-script-dialog').dialog('open');
    });
});

function saveScript() {

    var params = {
        Name: $('#save-script-name').val(),
        Description: $('#save-script-description').val(),
        Commands: _scriptContext
    };

    $.ajax({
        url: '/CaPSLOC/Script/Save',
        type: 'POST',
        dataType: 'json',
        data: JSON.stringify(params),
        contentType: 'application/json',
        success: function (result) {
            if (result.success) {
                alert('Script save successfully');
            } else {
                alert('An error occurred while saving the script: ' + result.data);
            }
        },
        error: function () {
            alert('An error occurred while saving the script');
        }
    });
}

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

function reloadScriptGrid() {

    var scriptDisplayData = { page: 1, total:0, rows: [] };

    $.each(_scriptContext, function (cIndex, cElement) {
        scriptDisplayData.rows.push({ id: cElement.CommandId, cell: [cElement.CommandName, null, null] });
        scriptDisplayData.total++;
        $.each(cElement.Params, function (pIndex, pElement) {
            scriptDisplayData.rows.push({ id: pElement.Id, cell: [null, pElement.Name, pElement.Value] });
            scriptDisplayData.total++;
        });
    });

    $('#script-display').flexAddData(scriptDisplayData);
}

function deleteScriptElement() {
    var selectedRow = $('#script-display .trSelected')
    var commandId = selectedRow.attr('id').substr(3);
    if (commandId.indexOf('-') != -1) {
        commandId = commandId.substr(0, commandId.indexOf('-'))
    }
    // remove command and associated parameters
    _scriptContext = $.grep(_scriptContext, function (element, index) {
        return element.CommandId != commandId
        });

    reloadScriptGrid();
}

function readScriptInput(cmdType) {
    var serializedInput = { CommandType: cmdType, Params: [] };
    serializedInput.CommandId = Math.floor(Math.random() * 0xFFFFFFFF).toString(16);

    switch (parseInt(cmdType)) {
        case 1:
            serializedInput.CommandName = 'Go To Location';
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-1',
                Name: 'Latitude',
                Value: $('#latitude-script-field').val()
            });
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-2',
                Name: 'Longitude',
                Value: $('#longitude-script-field').val()
            });
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-3',
                Name: 'Altitude',
                Value: $('#altitude-script-field').val()
            });
            break;
        case 2:
            serializedInput.CommandName = 'Go To Location';
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
            serializedInput.CommandName = 'Halt';
            break;
        case 4:
            serializedInput.CommandName = 'Pause';
            break;
        case 5:
            serializedInput.CommandName = 'Resume';
            break;
        case 6:
            serializedInput.CommandName = 'Image Capture';
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-1',
                Name: 'Image Type',
                Value: $('input[name=image-capture-type]:checked').val()
            });
            if ($('#image-capture-duration').val() && $('input[name=image-capture-type]:checked').val() == 2) {
                serializedInput.Params.push({
                    Id: serializedInput.CommandId + '-2',
                    Name: 'Duration',
                    Value: $('#image-capture-duration').val()
                });
            }
            if ($('#image-capture-frame-rate').val() && $('input[name=image-capture-type]:checked').val() == 2) {
                serializedInput.Params.push({
                    Id: serializedInput.CommandId + '-3',
                    Name: 'Frame Rate',
                    Value: $('#image-capture-frame-rate').val()
                });
            }
            if ($('#image-capture-resolution').val()) {
                serializedInput.Params.push({
                    Id: serializedInput.CommandId + '-4',
                    Name: 'Resolution',
                    Value: $('#image-capture-resolution').val()
                });
            }
            break;
        case 7:
            serializedInput.CommandName = 'Rotate';
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-1',
                Name: 'Direction',
                Value: $('#rotate-direction').val()
            });
            if ($('#rotate-degrees').val()) {
                serializedInput.Params.push({
                    Id: serializedInput.CommandId + '-2',
                    Name: 'Degrees',
                    Value: $('#rotate-degrees').val()
                });
            }
            break;
        case 8:
            serializedInput.CommandName = 'Wait';
            if ($('#wait-duration').val()) {
                serializedInput.Params.push({
                    Id: serializedInput.CommandId + '-1',
                    Name: 'Duration',
                    Value: $('#wait-duration').val()
                });
            }
            break;
        case 9:
            serializedInput.CommandName = 'Execute Script';
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-1',
                Name: 'Script Name',
                Value: $('#execute-name').val()
            });
            break;
        case 10:
            serializedInput.CommandName = 'Set Defaults';
            if ($('#default-rotate-degrees').val()) {
                serializedInput.Params.push({
                    Id: serializedInput.CommandId + '-1',
                    Name: 'Degrees',
                    Value: $('#default-rotate-degrees').val()
                });
            }
            if ($('#default-image-capture-duration').val()) {
                serializedInput.Params.push({
                    Id: serializedInput.CommandId + '-2',
                    Name: 'Video Duration',
                    Value: $('#default-image-capture-duration').val()
                });
            }
            if ($('#default-image-capture-frame-rate').val()) {
                serializedInput.Params.push({
                    Id: serializedInput.CommandId + '-3',
                    Name: 'Video Frame Rate',
                    Value: $('default-image-capture-frame-rate').val()
                });
            }
            if ($('#default-image-capture-resolution').val()) {
                serializedInput.Params.push({
                    Id: serializedInput.CommandId + '-4',
                    Name: 'Image Resolution',
                    Value: $('#default-image-capture-resolution').val()
                });
            }
            if ($('#default-wait-duration').val()) {
                serializedInput.Params.push({
                    Id: serializedInput.CommandId + '-5',
                    Name: 'Wait Duration',
                    Value: $('#default-wait-duration').val()
                });
            }
            if ($('#latitude-offset').val()) {
                serializedInput.Params.push({
                    Id: serializedInput.CommandId + '-6',
                    Name: 'Latitude Offset',
                    Value: $('#latitude-offset').val()
                });
            }
            if ($('#longitude-offset').val()) {
                serializedInput.Params.push({
                    Id: serializedInput.CommandId + '-7',
                    Name: 'Longitude Offset',
                    Value: $('#longitude-offset').val()
                });
            }
            if ($('#altitude-offset').val()) {
                serializedInput.Params.push({
                    id: serializedInput.CommandId + '-8',
                    Name: 'Altitude Offset',
                    Value: $('#altitude-offset').val()
                });
            }
            break;
        default:
            alert('Invalid command type');
            break;
    }
    return serializedInput;
}

function writeScriptInput(serializedInput) {

    var cmdType = serializedInput.commandType;

    switch (parseInt(cmdType)) {
        case 1:
            $.each(serializedInput.Params, function (index, element) {
                var paramType = element.Id.substr(element.Id.indexOf('-') + 1);
                if (paramType == '1') {
                    $('#latitude-script-field').val(element.Value);
                } else if (paramType == '2') {
                    $('#longitude-script-field').val(element.Value);
                } else if (paramType == '3') {
                    $('#altitude-script-field').val(element.Value);
                }
            });
            break;
        case 2:
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
        case 3:  // No parameters -> nothing to write
        case 4:
        case 5:
            break;
        case 6:
            $.each(serializedInput.Params, function (index, element) {
                var paramType = element.Id.substr(element.Id.indexOf('-') + 1);
                if (paramType == '1') {
                    $('input[name=image-capture-type]:checked').val(element.Value);
                } else if (paramType == '2') {
                    $('#image-capture-duration').val(element.Value);
                } else if (paramType == '3') {
                    $('image-capture-frame-rate').val(element.Value);
                } else if (paramType == '4') {
                    $('#image-capture-resolution').val(element.Value);
                }
            });
            break;
        case 7:
            $.each(serializedInput.Params, function (index, element) {
                var paramType = element.Id.substr(element.Id.indexOf('-') + 1);
                if (paramType == '1') {
                    $('#rotate-direction').val(element.Value);
                } else if (paramType == '2') {
                    $('#rotate-degrees').val(element.Value);
                }
            });
            break;
        case 8:
            $.each(serializedInput.Params, function (index, element) {
                var paramType = element.Id.substr(element.Id.indexOf('-') + 1);
                if (paramType == '1') {
                    $('#wait-duration').val(element.Value);
                }
            });
            break;
        case 9:
            $.each(serializedInput.Params, function (index, element) {
                var paramType = element.Id.substr(element.Id.indexOf('-') + 1);
                if (paramType == '1') {
                    $('#execute-name').val(element.Value);
                }
            });
            break;
        case 10:
            $.each(serializedInput.Params, function (index, element) {
                var paramType = element.Id.substr(element.Id.indexOf('-') + 1);
                if (paramType == '1') {
                    $('#default-rotate-degrees').val(element.Value);
                } else if (paramType == '2') {
                    $('#default-image-capture-duration').val(element.Value);
                } else if (paramType == '3') {
                    $('default-image-capture-frame-rate').val(element.Value);
                } else if (paramType == '4') {
                    $('#default-image-capture-resolution').val(element.Value);
                } else if (paramType == '5') {
                    $('#default-wait-duration').val(element.Value);
                } else if (paramType == '6') {
                    $('#latitude-offset').val(element.Value);
                } else if (paramType == '7') {
                    $('#longitude-offset').val(element.Value);
                } else if (paramType == '8') {
                    $('#altitude-offset').val(element.Value);
                }
            });
            break;
        default:
            alert('Invalid command type');
            break;
    }
}