<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<div class="container">
    <!-- Modal -->
    <!--    --><? //= form_open_multipart('/registro', 'id="formSignup", class="up modal fade"'); ?>
    <form id="formSignup" class="up modal fade " action="" method="post" role="dialog"
          enctype="multipart/form-data">
        <div class="modal-dialog">

            <!-- Modal content-->
            <div class="modal-content">
                <button type="button" class="close close-modal" data-dismiss="modal">&times;</button>
                <div class="modal-header">

                    <h4 class="modal-title text-center">Registrarse</h4>
                </div>
                <div class="modal-body">
                    <label for="control-label"><span class="glyphicon glyphicon-pencil"></span> Nombre</label>
                    <input type="text" class="text form-control" placeholder="Introduce un nombre" name="name" required>

                    <label for="control-label"><span class="glyphicon glyphicon-user"></span> Usuario</label>
                    <input type="text" class="text form-control" placeholder="Introduce un usuario" name="user"
                           required>

                    <label class="control-label" for="mail"><span class="glyphicon glyphicon-envelope"></span> Correo
                        electrónico</label>
                    <input type="text" class="text form-control" placeholder="Introduce un email" name="mail" required>

                    <label class="control-label" for="pass"><span class="glyphicon glyphicon-eye-close"></span>
                        Contraseña</label>
                    <input type="password" class="form-control" placeholder="Introduce contraseña" name="pass" required>

                    <label class="control-label" for="verify"><span class="glyphicon glyphicon-eye-open"></span> Repetir
                        contraseña</label>
                    <input type="password" class="form-control" placeholder="Repetir contraseña" name="pass-verify"
                           required>

                </div>
                <div id="AJAX"></div>
                <div class="modal-footer text-center">
                    <button type="button" data-dismiss="modal" class="btn btn-lg btn-danger">Cancelar</button>
                    <button type="button" id="submit-register" value="upload" class="btn btn-lg btn-success">Registrar
                    </button>

                </div>
            </div>

        </div>
    </form>
</div>
<script type='text/javascript'>
    /* attach a submit handler to the form */

    $(document).ready(function () {
        function registred() {
            var formData = {
                'name': $('input[name=name]').val(),
                'user': $('input[name=user]').val(),
                'mail': $('input[name=mail]').val(),
                'pass': $('input[name=pass]').val(),
                'pass-verify': $('input[name=pass-verify]').val()

            };
            $.ajax({
                data: formData,
                url: document.location.origin + '/registro',
                type: 'post',
                success: function (data) {
                    var status = JSON.parse(data).status;
                    switch(status){
                        case 200:
                            window.location.reload();
                            break;
                        case 416:
                            $('#AJAX').html('').append('El nombre de usuario o el email ya están registrados');
                            break;
                        case 418:
                            $('#AJAX').html('').append('Las contraseñas no coinciden');
                            break;
                    }

//                $("#formSignup").modal('hide');
                }

            })
        }
        $("#submit-register").click(function() {
            registred()
        });
        $('#formSignup').keypress(function (e) {
            if (e.which == 13) {
                registred();
            }
        });
    })

</script>
