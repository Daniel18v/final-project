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
                    <label for="user"><span class="glyphicon glyphicon-user"></span> Usuario</label>
                    <input type="text" class="text form-control" placeholder="Introduce un usuario" name="user" required>

                    <label for="pass"><span class="glyphicon glyphicon-eye-open"></span> Repetir contraseña</label>
                    <input type="password" class="form-control" placeholder="Introduce contraseña" name="pass" required>

                </div>
                <div class="modal-footer text-center">
                    <button type="button" data-dismiss="modal" class="btn btn-lg btn-danger">Cancelar</button>
                    <button type="submit" class=" btn btn-lg btn-info">Iniciar sesión</button>
                </div>
            </div>

        </div>
    </form>
</div>

<!--<div class="alert alert-danger" role="alert">-->
<!--    <span class="glyphicon glyphicon-exclamation-sign" aria-hidden="true"></span>-->
<!--    <span class="sr-only">Error:</span>-->
<!--    < ?= $this->session->flashdata('error') ?> -->
<!--</div>-->