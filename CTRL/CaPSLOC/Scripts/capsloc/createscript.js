
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
        var data = readScriptInput($('#command-list').val(), $('#create-script'));

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
