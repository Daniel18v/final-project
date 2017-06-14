<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>
<div class="container">
    <!-- Modal -->
    <!--    --><? //= form_open_multipart('/registro', 'id="formSignup", class="up modal fade"'); ?>
    <form id="formSignup" class="up modal fade " action="<?= site_url() . "registro"?> " method="post" role="dialog">
        <div class="modal-dialog">

            <!-- Modal content-->
            <div class="modal-content">
                <button type="button" class="close close-modal" data-dismiss="modal">&times;</button>
                <div class="modal-header">

                    <h4 class="modal-title text-center">Registrarse</h4>
                </div>
                <div class="modal-body">
                    <label for="control-label" for="name"><span class="glyphicon glyphicon-pencil"></span> Nombre</label>
                    <input type="text" minlength="3" maxlength="20" class="text form-control" placeholder="Introduce un nombre" name="name">


                    <label for="control-label" for="user"><span class="glyphicon glyphicon-user"></span> Nombre de usuario</label>
                    <input type="text" minlength="3" maxlength="20" class="text form-control" placeholder="Introduce un usuario" name="user"
                           required="true">

                    <label class="control-label" for="bornDate"><span class="fa fa-birthday-cake" aria-hidden="true"></span> Fecha de nacimiento</label>
                        <input type="date" class="text form-control" placeholder="Introduce la fecha de nacimiento" name="bornDate">

                    <label class="control-label" for="mail"><span class="glyphicon glyphicon-envelope"></span> Correo
                        electrónico</label>
                    <input type="email" class="text form-control" placeholder="Introduce un email" name="mail" required="true">

                    <label class="control-label" for="pass"><span class="glyphicon glyphicon-eye-close"></span>
                        Contraseña</label>
                    <input type="password" minlength="3" maxlength="20" class="form-control" placeholder="Introduce contraseña" name="pass" required="true">

                    <label class="control-label" for="verify"><span class="glyphicon glyphicon-eye-open"></span> Repetir
                        contraseña</label>
                    <input type="password" minlength="3" maxlength="20" class="form-control" placeholder="Repetir contraseña" name="pass-verify"
                           required="true">

                    <!--<input type="date" minlength="3" maxlength="20" class="form-control" placeholder="Repetir contraseña" name="pass-verify"-->
                    <!--       required="true">-->


                </div>
                <div id="AJAX"></div>
                <div class="modal-footer text-center">
                    <button type="button" data-dismiss="modal" class="btn btn-lg btn-danger">Cancelar</button>
                    <button type="submit" id="submit-register" value="upload" class="btn btn-lg btn-success">Registrar
                    </button>

                </div>
            </div>

        </div>
    </form>
</div>

<script type='text/javascript'>
        function signUp() {
            var formData = {
                'name': $('input[name=name]').val(),
                'user': $('input[name=user]').val(),
                'mail': $('input[name=mail]').val(),
                'pass': $('input[name=pass]').val(),
                'pass-verify': $('input[name=pass-verify]').val(),
                'bornDate': $('input[name=bornDate]').val(),

            };
            $.ajax({
                data: formData,
                url: document.location.origin + '/registro',
                type: 'post',
                success: function (data) {
                    var status = JSON.parse(data).status;
                    switch (status) {
                        case 200:
                            $('#AJAX').hide()
                            window.location.reload()
                            break;
                        case 414:
                            $('#AJAX').html("<div class='alert alert-danger text-center'>Rellena todos los campos</div>");
                            break;
                        case 415:
                            $('#AJAX').html("<div class='alert alert-danger text-center'>Las contraseñas no coinciden</div>");
                            break;
                        case 416:
                            $('#AJAX').html("<div class='alert alert-danger text-center'>El nombre de usuario ya está registrado</div>");
                            break;
                        case 417:
                            $('#AJAX').html("<div class='alert alert-danger text-center'>El correo ya está registrado</div>");
                            break;
                        case 419:
                            $('#AJAX').html("<div class='alert alert-danger text-center'>Algo ha fallado</div>");
                            break;
                        default :
                            $('#AJAX').html("<div class='alert alert-danger text-center'>Algo ha fallado</div>");
                            break;
                    }
                }
            })
        }

        $("#submit-register").click(function () {
            callSubmt()
        });
        $('#formSignup').keypress(function (e) {
            if (e.which == 13) {
                callSubmt()
            }
        });
        function callSubmt() {
            $("#formSignup").validate();
            $("#formSignup").on('submit', function (e) {
                if ($("#formSignup").valid()) {
                    e.preventDefault();
                    signUp();
                    checkSubmit()
                }
            });
        }

        var statSend = false;
        function checkSubmit() {
            if (!statSend) {
                statSend = true;
                return true;
            } else {
                return false;
            }
        }
</script>
