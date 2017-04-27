<body>
    <div class="container">
        <div class="col-md-12">

            <h1 class="page-header"><?= $title ?></h1>     
           
            <div class="table-responsive">
                <table class="table table-striped">
                    <thead>
                        <tr>   
                            <th>
                                Nombre de la consola 
                            </th>
                            <th>
                                Descripción
                            </th>

                        </tr>
                    </thead>
                    <tbody>
                        <?php if ($all !== FALSE): ?>
                            <?php foreach ($all as $all_titles): ?>
                                <tr>
                                    <td>
                                        <a href="<?= site_url("games/" . $all_titles['TITLE']) ?>">
                                            <?= $all_titles['TYPE'] ?>
                                        </a>
                                    </td> 
                                    <td><?= $all_titles['DESCRIPTION'] ?></td>

                                </tr>
                            <?php endforeach; ?>
                        <?php endif ?>
                </table> 
            </div>
        </div>
    </div>   
