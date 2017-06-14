$(document).ready(function () {
    var editor;
    editor = new $.fn.dataTable.Editor({
        ajax: 'index.php/ACP_controller/blog_actions',
        table: "#blogTable",
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
            label: "Imagen",
            name: "image"
        }, {
            label: "Fecha",
            name: "timestamp"
        }
        ]
    });

    //New record
    $('a.editor_create_blog').on('click', function (e) {
        e.preventDefault();

        editor.create({
            title: 'Crear nuevo registro',
            buttons: 'Añadir'
        });
    });

    // Edit record
    $('#blogTable').on('click', 'a.editor_edit_blog', function (e) {
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
    $('#blogTable').on('click', 'a.editor_remove_blog', function (e) {
        e.preventDefault();
        editor.message('¿Estás seguro de que quieres borrar esta entrada? ¿DE VERDAD?')
            .buttons({
                "label": "Borrar", "fn": function () {
                    editor.submit();
                }
            })
            .remove($(this).closest('tr'));
    });

    $('#blogTable').DataTable({
        responsive: true,
        dom: "Bfrtip",
        ajax: {
            url: "index.php/ACP_controller/blog_list",
            dataSrc: ''
        },
        columns: [
            {data: "id"},
            {data: "title"},
            {data: "slug"},
            {data: "description"},
            // {data: "publishing"},
            {data: "image"},
            {data: "timestamp"},
            {
                data: null,
                className: "center",
                defaultContent: '<a href="" class="editor_edit_blog"><i class="glyphicon glyphicon-pencil"></a></i> / <a href="" class="editor_remove_blog"><i class="glyphicon glyphicon-trash"></a>'
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
