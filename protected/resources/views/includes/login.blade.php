<div class="container">
    <!-- Modal -->
    <form id="formLogin" class="up modal fade " action="{{ url('login') }}" method="post" role="dialog">
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
                    <label for="user"><span class="glyphicon glyphicon-user"></span> Nombre de usuario</label>
                    <input type="text" class="text form-control" placeholder="Introduce un usuario" id="user"
                           minlength="3" maxlength="20" name="user" required>

                    <label for="pass"><span class="glyphicon glyphicon-eye-open"></span> Contraseña     &nbsp; </label>
                    <input type="password" class="form-control" placeholder="Introduce contraseña" id="pass" name="pass"
                           minlength="4" maxlength="20" required>

                </div>
                <div id="alert-error"></div>
                <div class="modal-footer text-center">
                    <button type="button" data-dismiss="modal" class="btn btn-lg btn-danger">Cancelar</button>
                    <button type="submit" id="submit-login" class=" btn btn-lg btn-info">Iniciar sesión</button>
                </div>
            </div>

        </div>
    </form>
</div>
<script type='text/javascript'>
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
                    case 418:
                        $('#alert-error').html("<div class='alert alert-danger text-center'>El usuario no está registrado</div>");
                        break;
                    case 420:
                        $('#alert-error').html("<div class='alert alert-danger text-center'>La contraseña no es correcta</div>")
                        break;
                }
            }
        })
    }
    $("#submit-login").click(function () {
        callSubmit()
    });
    $('#formLogin').keypress(function (e) {
        if (e.which == 13) {
            callSubmit()
        }
    });
    function callSubmit() {
        $("#formLogin").validate();
        $("#formLogin").on('submit', function (e) {
            if ($("#formLogin").valid()) {
                e.preventDefault();
                login();
            }
        });
    }
</script>