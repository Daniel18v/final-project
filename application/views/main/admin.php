<?php defined("BASEPATH") or exit("No direct script access allowed"); ?>

<?php $this->layout("templates::master") ?>
<?php $this->start('header') ?>
<?php $this->insert("structure::header", ['sess' => $sess]) ?>
<?php $this->stop() ?>

<?php $this->start('nav') ?>
<?php $this->insert("structure::nav") ?>
<?php $this->stop() ?>

<?php $this->start('section') ?>
    <div class="container-fluid wrapper">

        <div class="row">
            <div class="col-lg-10 col-lg-offset-1">
                <div class="panel panel-info">
                    <div class="panel-heading">
                        <h3 class="h3">Administrador de roms</h3>
                    </div>
                    <div class="panel-body">
                        <table id="romsTable" class="display" cellspacing="0" width="100%">
                            <thead>
                            <tr>
                                <th>ID</th>
                                <th>Título</th>
                                <th>Slug</th>
                                <th>Descripción</th>
                                <th>Tipo</th>
                                <th>Rom</th>
                                <th>Imagen</th>
                            </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <script type="text/javascript">
        $(document).ready(function () {
            $.ajax({
                url: 'index.php/ACP_controller/roms_list',
                type: 'POST',
                dataType: 'json',
                success: function (json) {
//                    console.log(json)
                    printResults(json)

                }
            });
            function printResults(results) {
                for (var numRs in results) {
                    var success = results[numRs];
                    $('#romsTable').DataTable().row.add([
                        success.id,
                        success.title,
                        success.slug,
                        success.description,
                        success.type,
                        success.rom,
                        success.image
                    ]).draw();
                }

            }

        });
    </script>

    <link rel="stylesheet" type="text/css" href="application/libraries/DataTables/media/css/jquery.dataTables.css">
    <link rel="stylesheet" type="text/css" href="application/libraries/DataTables/media/css/dataTables.editor.css">
    <script type="text/javascript" language="javascript"
            src="application/libraries/DataTables/media/js/jquery.js"></script>
    <script type="text/javascript" language="javascript"
            src="application/libraries/DataTables/media/js/jquery.dataTables.js"></script>
    <script type="text/javascript" language="javascript" src="application/libraries/DataTables/media/js/dataTables.tableTools.js"></script>
    <script type="text/javascript" language="javascript" src="application/libraries/DataTables/media/js/dataTables.editor.js"></script>
<?php $this->stop() ?>