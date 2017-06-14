<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>
<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>



<ul class="nav nav-tabs" role="tablist">
    <li class="active"><a href="#roms" aria-controls="roms" role="tab" data-toggle="tab">Roms</a></li>
    <li role="presentation"><a href="#usuarios" aria-controls="usuarios" role="tab" data-toggle="tab">Usuarios</a></li>
    <li role="presentation"><a href="#blog" aria-controls="blog" role="tab" data-toggle="tab">Blog</a></li>
</ul>
<div class="tab-content">
    <div role="tabpanel" class="tab-pane active" id="roms">
        <div class="row">
            <div class="col-lg-10 col-lg-offset-1">
                <div class="panel panel-info">
                    <div class="panel-heading">
                        <h3 class="h3">Administrador de roms</h3>
                    </div>
                    <a class="editor_create_rom">Crear nuevo registro</a>
                    <div class="panel-body">
                        <table id="romsTable" class="display" cellspacing="0" width="100%">
                            <thead>
                            <tr>
                                <th>ID</th>
                                <th>Título</th>
                                <th>Slug</th>
                                <th>Tipo</th>
                                <th>Acciones</th>
                            </tr>
                            </thead>
                            <tfoot>
                            <tr>
                                <th>ID</th>
                                <th>Título</th>
                                <th>Slug</th>
                                <th>Tipo</th>
                                <th>Acciones</th>
                            </tr>
                            </tfoot>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <div role="tabpanel" class="tab-pane" id="usuarios">
        <div class="row">
            <div class="col-lg-10 col-lg-offset-1">
                <div class="panel panel-info">
                    <div class="panel-heading">
                        <h3 class="h3">Administrador de usuarios</h3>
                    </div>
                    <a class="editor_create_user">Crear nuevo registro</a>
                    <div class="panel-body">
                        <table id="userTable" class="display" cellspacing="0" width="100%">
                            <thead>
                            <tr>
                                <th>ID</th>
                                <th>Rol</th>
                                <th>Nombre</th>
                                <th>Usuario</th>
                                <th>Correo</th>
                                <th>Birdcoins</th>
                                <th>Acciones</th>
                            </tr>
                            </thead>
                            <tfoot>
                            <tr>
                                <th>ID</th>
                                <th>Rol</th>
                                <th>Nombre</th>
                                <th>Usuario</th>
                                <th>Correo</th>
                                <th>Birdcoins</th>
                                <th>Acciones</th>
                            </tr>
                            </tfoot>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <div role="tabpanel" class="tab-pane" id="blog">
        <div class="row">
            <div class="col-lg-10 col-lg-offset-1">
                <div class="panel panel-info">
                    <div class="panel-heading">
                        <h3 class="h3">Administrador de entradas</h3>
                    </div>
                    <a href="<?= site_url() . "admin/blog" ?>">
                        <button class="btn btn-info">Añadir nueva entrada</button>
                    </a>
                    <div class="panel-body">
                        <table id="blogTable" class="display" cellspacing="0" width="100%">
                            <thead>
                            <tr>
                                <th>ID</th>
                                <th>Titulo</th>
                                <th>Slug</th>
                                <th>Descripción</th>
                                <th>Contenido</th>
                                <th>Imagen</th>
                                <th>Fecha</th>
                            </tr>
                            </thead>
                            <tfoot>
                            <tr>
                                <th>ID</th>
                                <th>Titulo</th>
                                <th>Slug</th>
                                <th>Descripción</th>
                                <th>Contenido</th>
                                <th>Imagen</th>
                                <th>Fecha</th>
                            </tr>
                            </tfoot>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<script type="text/javascript" language="javascript" src="/js/datatable-roms.js"></script>
<script type="text/javascript" language="javascript" src="/js/datatable-users.js"></script>
<script type="text/javascript" language="javascript" src="/js/datatable-blogs.js"></script>

<link rel="stylesheet" type="text/css" href="/application/libraries/DataTables/media/css/jquery.dataTables.css">
<link rel="stylesheet" type="text/css" href="/application/libraries/DataTables/media/css/editor.dataTables.min.css">
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/jquery.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/jquery.dataTables.min.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/dataTables.bootstrap.min.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/dataTables.buttons.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/dataTables.buttons.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/jquery.dataTables.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/dataTables.select.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/dataTables.tableTools.js"></script>
<script type="text/javascript" language="javascript"
        src="/application/libraries/DataTables/media/js/dataTables.editor.js"></script>
<?php $this->stop() ?>

