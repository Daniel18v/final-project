<div class="container">
    <!-- Modal -->
    <form id="formSignin" class="up modal fade " action="<?= site_url("/register") ?>" method="post" role="dialog">
        <div class="modal-dialog">

            <!-- Modal content-->
            <div class="modal-content">
                <button type="button" class="close" data-dismiss="modal">&times;</button>
                <div class="modal-header">

                    <h4 class="modal-title text-center">Registrarse</h4>
                </div>
                <div class="modal-body">
                    <label class="control-label" for="email"><span class="glyphicon glyphicon-envelope"></span> Correo electrónico</label>
                    <input type="text" class="text form-control" placeholder="Introduce un email" name="email" required>
                    <label class="control-label" for="psw"><span class="glyphicon glyphicon-eye-close"></span> Contraseña</label>
                    <input type="password" class="form-control" placeholder="Introduce contraseña" name="psw" required>
                    <label class="control-label" for="psw"><span class="glyphicon glyphicon-eye-open"></span> Repetir contraseña</label>
                    <input type="password" class="form-control" placeholder="Repetir contraseña" name="psw-repeat" required>
                </div>
                <div class="modal-footer text-center">
                    <button type="button" data-dismiss="modal" class="btn btn-lg btn-danger">Cancelar</button>
                    <button type="submit" class="btn btn-lg btn-success">Registrar</button>
                </div>
            </div>

        </div>
    </form>
</div>
