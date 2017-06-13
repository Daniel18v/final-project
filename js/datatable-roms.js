$(document).ready(function () {
    var editor;
    editor = new $.fn.dataTable.Editor({
        ajax: 'index.php/ACP_controller/rom_actions',
        table: "#romsTable",
        idSrc: 'id',
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
        }
        ]
    });

    //New record
    $('a.editor_create_rom').on('click', function (e) {
        e.preventDefault();

        editor.create({
            title: 'Crear nuevo registro',
            buttons: 'Añadir'
        });
    });

    // Edit record
    $('#romsTable').on('click', 'a.editor_edit_rom', function (e) {
        e.preventDefault();
        editor.title('Editar registro')
            .buttons({
                "label": "Actualizar", "fn": function () {
                    editor.submit();
                }
            })
            .edit($(this).closest('tr'));
    });

    // Delete a record
    $('#romsTable').on('click', 'a.editor_remove_rom', function (e) {
        e.preventDefault();
        editor.message('¿Estás seguro de que quieres borrar esta entrada? ¿DE VERDAD?')
            .buttons({
                "label": "Borrar", "fn": function () {
                    editor.submit();
                }
            })
            .remove($(this).closest('tr'));
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
            {data: "type"},
            {
                data: null,
                className: "center",
                defaultContent: '<a href="" class="editor_edit_rom"><i class="glyphicon glyphicon-pencil"></a></i> / <a href="" class="editor_remove_rom"><i class="glyphicon glyphicon-trash"></a>'
            }
        ],
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
});
