<div class="container">
    <!-- Modal -->
    <form id="formLogin" class="up modal fade " action="<?= site_url("/login") ?>" method="post" role="dialog">
        <div class="modal-dialog">

            <!-- Modal content-->
            <div class="modal-content">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span>
                </button>
                <div class="modal-header">

                    <h4 class="modal-title text-center">Iniciar sesión</h4>
                </div>
                <div class="modal-body">
                    <label for="email"><span class="glyphicon glyphicon-envelope"></span> Correo electrónico</label>
                    <input type="text" class="text" placeholder="Introduce un email" name="email" required>

                    <label for="psw"><span class="glyphicon glyphicon-eye-open"></span> Repetir contraseña</label>
                    <input type="password" placeholder="Introduce contraseña" name="psw" required>

                </div>
                <div class="modal-footer text-center">
                    <button type="button" data-dismiss="modal" class="btn btn-lg btn-danger">Cancelar</button>
                    <button type="submit" class=" btn btn-lg btn-info">Iniciar sesión</button>
                </div>
            </div>

        </div>
    </form>
</div>
