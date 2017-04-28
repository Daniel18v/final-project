<?php $this->insert("structure::header") ?>
<div class="container-fluid">
    <section class="section">
        <div class="row">
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
                                            <a href="<?= site_url("games/" . $all_titles['title']) ?>">
                                                <?= $all_titles['type'] ?>
                                            </a>
                                        </td> 
                                        <td><?= $all_titles['description'] ?></td>

                                    </tr>
                                <?php endforeach; ?>
                            <?php endif ?>
                    </table> 
                </div>
            </div>
        </div>

    </section>
</div>   
<?php $this->insert("structure::footer") ?>