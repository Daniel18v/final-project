<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>
<?php require("application/models/ACP_model.php") ?>
<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>

    <div class="hr-margin-bottom-10"></div>

    <div class="row">

        <div class="col-lg-6 col-lg-offset-3">
            <div class="panel panel-info">
                <div class="panel-heading text-center">
                    <h3 class="panel-title">Administrador de roms</h3>
                    <table id="adminRoms" class="display" cellspacing="0" width="100%">

                        <thead>
                        <tr>
                            <th>id</th>
                            <th>title</th>
                            <th>slug</th>
                            <th>description</th>
                            <th>type</th>
                            <th>rom</th>
                            <th>images</th>
                        </tr>
                        </thead>

                        <tfoot>
                        <tr>
                            <th>id</th>
                            <th>title</th>
                            <th>slug</th>
                            <th>description</th>
                            <th>type</th>
                            <th>rom</th>
                            <th>images</th>
                        </tr>
                        </tfoot>

                    </table>
                </div>
            </div>
        </div>

    </div>
    <script>
        $(document).ready(function () {
            var datatable = $('#adminRoms').DataTable({
                ajax: "selectData()",
                columns: [
                    {data: "id"},
                    {data: "title"},
                    {data: "slug"},
                    {data: "description"},
                    {data: "type"},
                    {data: "rom"},
                    {data: "images"}
                ]
            });
        });
    </script>
    <link rel="stylesheet" type="text/css" href="application/libraries/DataTables/media/css/jquery.dataTables.css">
    <link rel="stylesheet" type="text/css" href="css/dataTables.editor.css">
    <script type="text/javascript" language="javascript"
            src="application/libraries/DataTables/media/js/jquery.js"></script>
    <script type="text/javascript" language="javascript"
            src="application/libraries/DataTables/media/js/jquery.dataTables.js"></script>
    <script type="text/javascript" language="javascript" src="js/dataTables.tableTools.js"></script>
    <script type="text/javascript" language="javascript" src="js/dataTables.editor.js"></script>
<?php $this->stop() ?>