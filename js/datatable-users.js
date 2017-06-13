$(document).ready(function () {
    var editor;
    editor = new $.fn.dataTable.Editor({
        ajax: 'index.php/ACP_controller/user_actions',
        table: "#userTable",
        idSrc: 'id',
        fields: [{
            label: "ID",
            name: "id"
        },{
            label: "Rol",
            name: "rol"
        }, {
            label: "Nombre",
            name: "name"
        }, {
            label: "Usuario",
            name: "user"
        }, {
            label: "Correo",
            name: "mail",
        }, {
            label: "Birdcoins",
            name: "bird_coins",
        }
        ]
    });

    //New record
    $('a.editor_create_user').on('click', function (e) {
        e.preventDefault();

        editor.create({
            title: 'Crear nuevo registro',
            buttons: 'Añadir'
        });
    });

    // Edit record
    $('#userTable').on('click', 'a.editor_edit', function (e) {
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
    $('#userTable').on('click', 'a.editor_remove', function (e) {
        e.preventDefault();
        editor.message('¿Estás seguro de que quieres borrar esta entrada? ¿DE VERDAD?')
            .buttons({
                "label": "Borrar", "fn": function () {
                    editor.submit();
                }
            })
            .remove($(this).closest('tr'));
    });

    $('#userTable').DataTable({
        responsive: true,
        dom: "Bfrtip",
        ajax: {
            url: "index.php/ACP_controller/user_list",
            dataSrc: ''
        },
        columns: [
            {data: "id"},
            {data: "rol"},
            {data: "name"},
            {data: "user"},
            {data: "mail"},
            {data: "bird_coins"},
            {
                data: null,
                className: "center",
                defaultContent: '<a href="" class="editor_edit"><i class="glyphicon glyphicon-pencil"></a></i> / <a href="" class="editor_remove"><i class="glyphicon glyphicon-trash"></a>'
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
