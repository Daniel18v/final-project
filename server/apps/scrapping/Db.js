function Db(title, descript, imgs) {
    this.title = title;
    this.descript = descript;
    this.imgs = imgs;
    console.log(this.imgs[0]);
    this.add = function () {
        var mysql = require('mysql');
        
        var connection = mysql.createConnection({
            host: 'localhost'
            , user: 'root'
            , password: ''
            , database: 'freebird'
            , port: 80
        });
        /*
        connection.connect(function (error) {
            if (error) {
                console.log('Error.');
            }
            else {
                console.log('Conexion correcta.');
            }
        });
        connection.end();
        */
    }
}
/*   var mysql = require('mysql');
    var client = mysql.createClient({
        user: 'root'
        , password: ''
        , host: 'localhost'
        , port: '80'
    , });
    client.query('USE prueba');
    client.query('INSERT INTO usuario SET nombre = ?, password = ?', ['carlosro_ec', 'miclave']);
    client.query('SELECT * FROM usuario', function selectUsuario(err, results, fields) {
        if (err) {
            console.log("Error: " + err.message);
            throw err;
        }
        console.log("Number of rows: " + results.length);
        console.log(results);
        client.end();
    });
}*/
module.exports = Db;