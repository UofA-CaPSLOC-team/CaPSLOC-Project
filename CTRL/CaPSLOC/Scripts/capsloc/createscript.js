
var _scriptContext = [];

$(document).ready(function () {
    $('#command-list').change(function () {
        $('.command-parameters').css('display', 'none');
        var selectVal = $('#command-list').val();
        $('#command-parameters-' + selectVal).css('display', 'block');
    });

    scriptRefreshLocations();

    $('#script-display').flexigrid({
        colModel: [
            { display: 'Command', name: 'command', width: 150, sortable: false, align: 'left' },
            { display: 'Parameter Name', name: 'param_name', width: 150, sortable: false, align: 'left' },
            { display: 'Parameter Value', name: 'param_value', width: 150, sortable: false, align: 'left' },
        ],
        dataType: 'json',
        buttons: [{
            name: 'Delete',
            onpress: deleteScriptElement,
            bclass: 'edit'
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

    // Remove the unobtrusive validator so we can manually set up validation
    $('#create-script-add-form').removeData('validator');
    $('#create-script-add-form').validate({
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
        errorContainer: '#create-script-add-error',
        errorLabelContainer: '#create-script-add-error ul',
        wrapper: 'li'
    });

    $('#add-script-element').button().click(function () {
        if($('#create-script-add-form').valid()){
            var data = readScriptInput($('#command-list').val(), $('#create-script'));

            _scriptContext.push(data);

            reloadScriptGrid();
        }
    });

    $('#save-script').button().click(function () {
        $('#save-script-dialog').dialog('open');
    });
});

function scriptRefreshLocations(){
    var $locs = $('#goto-location-list');
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
                alert('An error occurred while loading locations: ' + result.data);
            }
        },
        error: function () {
            alert('An error occurred while loading locations');
        }
    });
}

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
                alert('Script saved successfully');
            } else {
                alert('An error occurred while saving the script: ' + result.data);
            }
        },
        error: function () {
            alert('An error occurred while saving the script');
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
