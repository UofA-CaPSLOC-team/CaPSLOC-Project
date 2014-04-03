_scriptList = [];

$(document).ready(function () {

    $('#add-script-dialog').dialog({
        autoOpen: false,
        height: 300,
        width: 350,
        title: 'Add Script',
        modal: true,
        buttons: {
            Add: addAltScript,
            Cancel: function () {
                $(this).dialog('close');
            }
        },
        open: function () {
            var $nameList = $('#add-script-name');
            $nameList.empty();
            $.ajax({
                url: '/CaPSLOC/Script/All',
                type: 'GET',
                success: function (result) {
                    if (result.success) {
                        $.each(result.data, function (index, element) {
                            $('<option/>').val(element.Id).text(element.Name).appendTo($nameList);
                        });
                    }
                }
            });

            var currentAltName = $('#manage-alt-list :selected').text();
            $('#add-script-alt').text(currentAltName);
        }
    });

    $('#manage-scripts-found').flexigrid({
        colModel: [
            { display: 'Script Name', name: 'script', width: 150, sortable: false, align: 'left' },
            { display: 'Modified', name: 'param_name', width: 150, sortable: false, align: 'left' }
        ],
        dataType: 'json',
        buttons: [{
            name: 'Add',
            onpress: function () {
                $('#add-script-dialog').dialog('open');
            },
            bclass: 'add'
        }, {
            name: 'Delete',
            onpress: deleteAltScript,
            bclass: 'delete'
        }],
        title: 'Available Scripts',
        width: 450,
        height: 300,
        singleSelect: true
    });

    $('#get-scripts-button').button().click(function () {
        var altId = $('#manage-alt-list').val();
        $.ajax({
            url: '/CaPSLOC/ALT/Scripts',
            type: 'GET',
            data: { 'altId': altId },
            success: function (result) {
                if (result.success) {
                    refreshScriptList(result.data);
                } else {
                    alert('An error occurred while finding the scripts: ' + result.data);
                }
            },
            error: function () {
                alert('An error occurred while finding the scripts');
            }
        });
    });
});

function manageRefreshALTs() {
    var $altList = $('#manage-alt-list');
    $altList.empty();
    $.ajax({
        url: '/CaPSLOC/ALT/RecentlyLocated',
        type: 'GET',
        success: function (result) {
            if (result.success) {
                $.each(result.data, function (index, element) {
                    $('<option/>').val(element.Id).text(element.Name + ' (' + element.Address + ')').appendTo($altList);
                });
            } else {
                alert('An error occurred while finding the ALTs: ' + result.data);
            }
        },
        error: function () {
            alert('An error occurred while finding the ALTs');
        }
    });
}

function addAltScript() {
    $('#add-script-dialog').dialog('close');

    var altId = $('#manage-alt-list').val();
    var scriptId = $('#add-script-name').val();

    $.ajax({
        url: '/CaPSLOC/Script/AddToALT',
        type: 'POST',
        dataType: 'json',
        data: { 'altId': altId, 'scriptId': scriptId },
        success: function (result) {
            if (result.success) {
                alert('Script added successfully');
                refreshScriptList(result.data);
            } else {
                alert('An error occurred while adding the script: ' + result.data);
            }
        },
        error: function () {
            alert('An error occurred while adding the script');
        }
    });
}

function deleteAltScript() {

    var selectedRows = $('#manage-scripts-found .trSelected');
    if (selectedRows.length > 0) {
        var index = selectedRows.attr('id').substr(3);
        var scriptName = _scriptList[index].Name;
        var altId = $('#manage-alt-list').val();

        $.ajax({
            url: '/CaPSLOC/Script/DeleteFromALT',
            type: 'POST',
            dataType: 'json',
            data: { 'altId': altId, 'scriptName': scriptName },
            success: function (result) {
                if (result.success) {
                    alert('Script deleted successfully');
                    refreshScriptList(result.data);
                } else {
                    alert('An error occurred while deleting the script: ' + result.data);
                }
            },
            error: function () {
                alert('An error occurred while deleting the script');
            }
        });
    }
    else {
        alert('Please select a row!');
    }
}

function refreshScriptList(scripts) {
    var newData = { page: 1, total: scripts.length, rows: [] };
    _scriptList = scripts;
    $.each(scripts, function (index, element) {
        newData.rows.push({ id: index.toString(), cell: [element.Name, new Date(parseInt(element.Modified.substr(6))).toLocaleString()] });
    });
    $('#manage-scripts-found').flexAddData(newData);
}