<!DOCTYPE html>
<html>
    <head>
        <title>Pruebas</title>
    </head>
    <body>
        <table>
            <tr>
                <th>Título</th>
                <th>Descripción</th>
                <th>Tipo</th>
                <th>Rom</th>
            </tr>
            <?php foreach ($articles as $row): ?>
            <tr>
                <td><?= $row['TITLE'];?></td>
                <td><?= $row['DESCRIPTION'];?></td>
                <td><?= $row['TYPE'];?></td>
                <td><?= $row['ROM'];?></td>
            </tr>
            <?php endforeach ?>
        </table>
    </body>
</html>
