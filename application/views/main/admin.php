<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>
<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>

<a href="<?= site_url() . "admin/blog" ?>"><button class="btn btn-info">Administrar Blog</button></a>


<div class="row">
    <div class="col-lg-10 col-lg-offset-1">
        <div class="panel panel-info">
            <div class="panel-heading">
                <h3 class="h3">Administrador de roms</h3>
            </div>
            <a class="editor_create">Crear nuevo registro</a>
            <div class="panel-body">
                <table id="romsTable" class="display" cellspacing="0" width="100%">
                    <thead>
                    <tr>
                        <th>ID</th>
                        <th>Título</th>
                        <th>Slug</th>
                        <th>Descripción</th>
                        <th>Tipo</th>
                        <th>Rom</th>
                        <th>Imagen</th>
                        <th>Editar/Borrar</th>
                    </tr>
                    </thead>
                    <tbody></tbody>
                </table>
            </div>
        </div>
    </div>
</div>
</div>
<script type="text/javascript">
//    var editor; // use a global for the submit and return data rendering in the examples
//
//    $(document).ready(function() {
//        editor = new $.fn.dataTable.Editor( {
//            ajax: "index.php/ACP_controller/roms_list",
//            table: "#example",
//            fields: [{
//                label: "ID",
//                name: "id"
//            }, {
//                label: "Título",
//                name: "title"
//            }, {
//                label: "Slug",
//                name: "slug"
//            }, {
//                label: "Descripción",
//                name: "description"
//            }, {
//                label: "Tipo",
//                name: "type"
//            }, {
//                label: "Rom",
//                name: "rom",
//            }, {
//                label: "Imagen",
//                name: "image",
//                type: "datetime"
//
//            }
//            ]
//        } );
//
//        var table = $('#romsTable').DataTable( {
//            lengthChange: false,
//            ajax: "index.php/ACP_controller/roms_list",
//            columns: [
//                {data: "id"},
//                {data: "title"},
//                {data: "slug"},
//                {data: "description"},
//                {data: "type"},
//                {data: "rom"},
//                {data: "image"},
//                {
//                    data: null,
//                    className: "center",
//                    defaultContent: '<a href="" class="editor_edit"><i class="glyphicon glyphicon-pencil"></a></i> / <a href="" class="editor_remove"><i class="glyphicon glyphicon-trash"></a>'
//                }
//            ],
//            select: true
//        } );
//
//        // Display the buttons
//        new $.fn.dataTable.Buttons( table, [
//            { extend: "create", editor: editor },
//            { extend: "edit",   editor: editor },
//            { extend: "remove", editor: editor }
//        ] );
//
//        table.buttons().container()
//            .appendTo( $('.col-sm-6:eq(0)', table.table().container() ) );
//    } );


    var editor;
    $(document).ready(function () {
        editor = new $.fn.dataTable.Editor({
            ajax: 'index.php/ACP_controller/roms_list',
            table: "#romsTable",
            idSrc:  'id',
            fields: [{
                label: "ID",
                name: "id"
            }, {
                label: "Título",
                name: "title"
            }, {
                label: "Slug",
                name: "slug"
            }, {
                label: "Descripción",
                name: "description"
            }, {
                label: "Tipo",
                name: "type"
            }, {
                label: "Rom",
                name: "rom",
            }, {
                label: "Imagen",
                name: "image",
                type: "datetime"

            }
            ]
        });


         //New record
        $('a.editor_create').on('click', function (e) {
            e.preventDefault();

            editor.create( {
                title: 'Crear nuevo registro',
                buttons: 'Añadir'
            } );
        } );

        // Edit record
        $('#romsTable').on('click', 'a.editor_edit', function (e) {
            e.preventDefault();

            editor.edit( $(this).closest('tr'), {
                title: 'Editar registro',
                buttons: 'Actualizar'
            } );
        } );

        // Delete a record
        $('#romsTable').on('click', 'a.editor_remove', function (e) {
            e.preventDefault();

            editor.remove( $(this).closest('tr'), {
                title: 'Borrar registro',
                message: '¿Estás seguro de que quieres borrar esta entrada? ¿DE VERDAD?',
                buttons: 'Borrar'
            } );
        } );


        // Activate an inline edit on click of a table cell
        $('#romsTable').on('click', 'tbody td', function () {
            editor.inline(this);
        });




        $('#romsTable').DataTable({
            responsive: true,
            dom: "Bfrtip",
            ajax: {
                url: "index.php/ACP_controller/roms_list",
                dataSrc: ''

            },
            columns: [

                {data: "id"},
                {data: "title"},
                {data: "slug"},
                {data: "description"},
                {data: "type"},
                {data: "rom"},
                {data: "image"},
                {
                    data: null,
                    className: "center",
                    defaultContent: '<a href="" class="editor_edit"><i class="glyphicon glyphicon-pencil"></a></i> / <a href="" class="editor_remove"><i class="glyphicon glyphicon-trash"></a>'
                }
            ],
            select: true,
            buttons: {
                name: 'primary',
                buttons: [ 'copy', 'csv', 'excel' ]
            },
            language: {

                "sProcessing": "Procesando...",
                "sLengthMenu": "Mostrar _MENU_ registros",
                "sZeroRecords": "No se encontraron resultados",
                "sEmptyTable": "Ningún dato disponible en esta tabla",
                "sInfo": "Mostrando registros del _START_ al _END_ de un total de _TOTAL_ registros",
                "sInfoEmpty": "Mostrando registros del 0 al 0 de un total de 0 registros",
                "sInfoFiltered": "(filtrado de un total de _MAX_ registros)",
                "sInfoPostFix": "",
                "sSearch": "Buscar:",
                "sUrl": "",
                "sInfoThousands": ",",
                "sLoadingRecords": "Cargando...",
                "oPaginate": {
                    "sFirst": "Primero",
                    "sLast": "Último",
                    "sNext": "Siguiente",
                    "sPrevious": "Anterior"
                },
                "oAria": {
                    "sSortAscending": ": Activar para ordenar la columna de manera ascendente",
                    "sSortDescending": ": Activar para ordenar la columna de manera descendente"
                }

            }
        });
//        $('#romsTable').Tabledit({
//            url: "index.php/ACP_controller/roms_update",
//            columns: {
//                identifier: [0, 'ID'],
//                editable: [[1, 'title'], [2, 'slug'], [3, 'description'], [4, 'type'], [5, 'rom'], [6, 'image']]
//            }
//
//        });
    });


</script>

<link rel="stylesheet" type="text/css" href="/application/libraries/DataTables/media/css/jquery.dataTables.css">
<link rel="stylesheet" type="text/css" href="/application/libraries/DataTables/media/css/editor.dataTables.min.css">
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/jquery.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/jquery.dataTables.min.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/dataTables.bootstrap.min.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/dataTables.buttons.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/dataTables.buttons.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/jquery.dataTables.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/dataTables.select.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/dataTables.tableTools.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/dataTables.editor.js"></script>
<?php $this->stop() ?>

$(document).ready(function () {
$.ajax({
url: 'index.php/ACP_controller/roms_list',
type: 'POST',
dataType: 'json',
success: function (json) {
//                    console.log(json)
printResults(json)

}
});
function printResults(results) {
for (var numRs in results) {
var success = results[numRs];
$('#romsTable').DataTable().row.add([
success.id,
success.title,
success.slug,
success.description,
success.type,
success.rom,
success.image
]).draw();
}
}
