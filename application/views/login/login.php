<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<div class="container">
    <!-- Modal -->
    <form id="formLogin" class="up modal fade " action="" method="post" role="dialog">
        <div class="modal-dialog">

            <!-- Modal content-->
            <div class="modal-content">
                <button type="button" class="close close-modal" data-dismiss="modal" aria-label="Close"><span
                            aria-hidden="true">&times;</span>
                </button>
                <div class="modal-header">

                    <h4 class="modal-title text-center">Iniciar sesión</h4>
                </div>
                <div class="modal-body">
                    <label for="user"><span class="glyphicon glyphicon-user"></span> Usuario</label>
                    <input type="text" class="text form-control" placeholder="Introduce un usuario" id="user"
                           name="user" required>

                    <label for="pass"><span class="glyphicon glyphicon-eye-open"></span> Repetir contraseña</label>
                    <input type="password" class="form-control" placeholder="Introduce contraseña" id="pass" name="pass"
                           required>

                </div>
                <div id="alert-error"></div>
                <div class="modal-footer text-center">
                    <button type="button" data-dismiss="modal" class="btn btn-lg btn-danger">Cancelar</button>
                    <button type="button" id="submit-login" class=" btn btn-lg btn-info">Iniciar sesión</button>
                </div>
            </div>

        </div>
    </form>
</div>
<script type='text/javascript'>
    /* attach a submit handler to the form */
    function login() {
        var formData = {
            'user': $("#user").val(),
            'pass': $("#pass").val()
        };
        $.ajax({
            data: formData,
            url: document.location.origin + '/login',
            type: 'post',
            success: function (data) {
                var status = JSON.parse(data).status;
                switch (status) {
                    case 200:
                        window.location.reload();
                        break;
                    case 417:
                        $('#alert-error').html("<div class='alert alert-danger text-center'>La contraseña no es correcta</div>");
                        break;
                    case 420:
                        $("#formLogin").modal('hide');
                        $("#formSignup").modal();
                        break;
                }

//                $("#formSignup").modal('hide');
            }

        })
    }
    $("#submit-login").click(function () {
        login();
    });
    $('#formLogin').keypress(function (e) {
        if (e.which == 13) {
            login();
        }
    });
</script>