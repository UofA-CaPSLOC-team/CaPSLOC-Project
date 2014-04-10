
var _debugRefreshId;

$(document).ready(function () {

    var tabs = $('#tabs');
    tabs.tabs({
        beforeActivate: function (event, ui) {
            switch ($(ui.newPanel).attr('id')) {
                case 'interactive-mode':
                    interactiveRefreshLocations();
                    interactiveRefreshALTs();
                    debugRefresh();
                    break;
                case 'create-script':
                    scriptRefreshLocations();
                    break;
                case 'manage-scripts':
                    manageRefreshALTs();
                    break;
                case 'image-gallery':
                    imageRefreshALTs();
                    imageRefreshLocations();
                    break;
                default:
                    break;
            }
        },
        activate: function (event, ui) {  // This one has to be done after the panel is displayed
            if (ui.newPanel.attr('id') == 'map-page') {
                initializeMap();
            }
            if (ui.oldPanel.attr('id') == 'interactive-mode') {
                clearTimeout(_debugRefreshId);  // Don't keep refreshing when we're not on the page
            }
        }
    });

    $('#wait-dialog').dialog({
        autoOpen: false,
        height: 200,
        width: 250,
        title: 'Please Wait...',
        modal: true
    });

    $('#wait-progress').progressbar({
        value: false
    });
});

function openWaitDialog() {
    $('#wait-dialog').dialog('open');
}

function closeWaitDialog() {
    $('#wait-dialog').dialog('close');
}

function readScriptInput(cmdType, tab) {
    var serializedInput = { CommandType: cmdType, Params: [] };
    serializedInput.CommandId = Math.floor(Math.random() * 0xFFFFFFFF).toString(16);

    switch (parseInt(cmdType)) {
        case 1:
            serializedInput.CommandName = 'Go To Location';
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-1',
                Name: 'Latitude',
                Value: $('[id$=latitude-script-field]', tab).val()
            });
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-2',
                Name: 'Longitude',
                Value: $('[id$=longitude-script-field]', tab).val()
            });
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-3',
                Name: 'Altitude',
                Value: $('[id$=altitude-script-field]', tab).val()
            });
            break;
        case 2:
            serializedInput.CommandName = 'Go To Location';
            var locText = $('[id$=goto-location-list]', tab).text();
            var locName = locText.substr(0, locText.indexOf('('));
            var locArray = locText.substring(locText.indexOf('(') + 1, locText.indexOf(')')).split(',');

            serializedInput.Params.push({
                id: serializedInput.CommandId + '-1',
                name: 'Latitude',
                value: $.trim(locArray[0])
            });
            serializedInput.Params.push({
                id: serializedInput.CommandId + '-2',
                name: 'Longitude',
                value: $.trim(locArray[1])
            });
            serializedInput.Params.push({
                id: serializedInput.CommandId + '-3',
                name: 'Altitude',
                value: $.trim(locArray[2])
            });
            serializedInput.Params.push({
                id: serializedInput.CommandId + '-4',
                name: 'Name',
                value: $.trim(locName)
            });
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
                Value: $('input[name$=image-capture-type]:checked', tab).val()
            });
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-2',
                Name: 'Duration',
                Value: 0 //$('[id$=image-capture-duration]', tab).val()
            });
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-3',
                Name: 'Frame Rate',
                Value: 10 //$('[id$=image-capture-frame-rate]', tab).val()
            });           
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-4',
                Name: 'Resolution',
                Value: $('[id$=image-capture-resolution]', tab).val() || 480
            });
            break;
        case 7:
            serializedInput.CommandName = 'Rotate';
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-1',
                Name: 'Direction',
                Value: $('[id$=rotate-direction]', tab).val()
            });
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-2',
                Name: 'Degrees',
                Value: $('[id$=rotate-degrees]', tab).val()
            });
            break;
        case 8:
            serializedInput.CommandName = 'Wait';
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-1',
                Name: 'Duration',
                Value: $('[id$=wait-duration]', tab).val()
            });
            break;
        case 9:
            serializedInput.CommandName = 'Execute Script';
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-1',
                Name: 'Script Name',
                Value: $('[id$=execute-name]', tab).val()
            });
            break;
        case 10:
            serializedInput.CommandName = 'Set Defaults';
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-1',
                Name: 'Degrees',
                Value: $('[id$=default-rotate-degrees]', tab).val() || 5
            });
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-2',
                Name: 'Video Duration',
                Value: $('[id$=default-image-capture-duration]', tab).val() || "00:00"
            });
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-3',
                Name: 'Video Frame Rate',
                Value: $('[id$=default-image-capture-frame-rate]', tab).val() || 5
            });
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-4',
                Name: 'Image Resolution',
                Value: $('[id$=default-image-capture-resolution]', tab).val() || 600
            });
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-5',
                Name: 'Wait Duration',
                Value: $('[id$=default-wait-duration]').val() || "00:05"
            });
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-6',
                Name: 'Latitude Offset',
                Value: $('[id$=latitude-offset]', tab).val() || 0
            });
            serializedInput.Params.push({
                Id: serializedInput.CommandId + '-7',
                Name: 'Longitude Offset',
                Value: $('[id$=longitude-offset]', tab).val() || 0
            });
            serializedInput.Params.push({
                id: serializedInput.CommandId + '-8',
                Name: 'Altitude Offset',
                Value: $('[id$=altitude-offset]', tab).val() || 0
            });
            break;
        default:
            alert('Invalid command type');
            break;
    }
    return serializedInput;
}

function writeScriptInput(serializedInput, tab) {

    var cmdType = serializedInput.commandType;

    switch (parseInt(cmdType)) {
        case 1:
            $.each(serializedInput.Params, function (index, element) {
                var paramType = element.Id.substr(element.Id.indexOf('-') + 1);
                if (paramType == '1') {
                    $('[id$=latitude-script-field]', tab).val(element.Value);
                } else if (paramType == '2') {
                    $('[id$=longitude-script-field]', tab).val(element.Value);
                } else if (paramType == '3') {
                    $('[id$=altitude-script-field]', tab).val(element.Value);
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
                    $('input[name$=image-capture-type]:checked', tab).val(element.Value);
                } else if (paramType == '2') {
                    $('[id$=image-capture-duration]', tab).val(element.Value);
                } else if (paramType == '3') {
                    $('[id$=image-capture-frame-rate]', tab).val(element.Value);
                } else if (paramType == '4') {
                    $('[id$=image-capture-resolution]', tab).val(element.Value);
                }
            });
            break;
        case 7:
            $.each(serializedInput.Params, function (index, element) {
                var paramType = element.Id.substr(element.Id.indexOf('-') + 1);
                if (paramType == '1') {
                    $('[id$=rotate-direction]', tab).val(element.Value);
                } else if (paramType == '2') {
                    $('[id$=rotate-degrees]', tab).val(element.Value);
                }
            });
            break;
        case 8:
            $.each(serializedInput.Params, function (index, element) {
                var paramType = element.Id.substr(element.Id.indexOf('-') + 1);
                if (paramType == '1') {
                    $('[id$=wait-duration]', tab).val(element.Value);
                }
            });
            break;
        case 9:
            $.each(serializedInput.Params, function (index, element) {
                var paramType = element.Id.substr(element.Id.indexOf('-') + 1);
                if (paramType == '1') {
                    $('[id$=execute-name]', tab).val(element.Value);
                }
            });
            break;
        case 10:
            $.each(serializedInput.Params, function (index, element) {
                var paramType = element.Id.substr(element.Id.indexOf('-') + 1);
                if (paramType == '1') {
                    $('[id$=default-rotate-degrees]', tab).val(element.Value);
                } else if (paramType == '2') {
                    $('[id$=default-image-capture-duration]', tab).val(element.Value);
                } else if (paramType == '3') {
                    $('[id$=default-image-capture-frame-rate]', tab).val(element.Value);
                } else if (paramType == '4') {
                    $('[id$=default-image-capture-resolution]', tab).val(element.Value);
                } else if (paramType == '5') {
                    $('[id$=default-wait-duration]', tab).val(element.Value);
                } else if (paramType == '6') {
                    $('[id$=latitude-offset]', tab).val(element.Value);
                } else if (paramType == '7') {
                    $('[id$=longitude-offset]', tab).val(element.Value);
                } else if (paramType == '8') {
                    $('[id$=altitude-offset]', tab).val(element.Value);
                }
            });
            break;
        default:
            alert('Invalid command type');
            break;
    }
}