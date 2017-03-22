var Db = require('./Db.js');
var casper = require('casper').create({
    clientScripts: ['lib/jquery.min.js']
});
var x = require("casper").selectXPath;
casper.start("https://www.emuparadise.me");
var system = require('system');
casper.then(function () {
    this.echo('1. Super_Nintendo_Entertainment_System_(SNES)_ROMs\n' + '2. Nintendo_Gameboy_Advance_ROMs');
    var rs = system.stdin.readLine();
    var url;
    var console;
    if (rs == '1') {
        url = "https://www.emuparadise.me/Super_Nintendo_Entertainment_System_(SNES)_ROMs/List-All-Titles/5";
    }
    else if (rs == '2') {
        url = "https://www.emuparadise.me/Nintendo_Gameboy_Advance_ROMs/List-All-Titles/31";
    }
    else {
        url = "https://www.emuparadise.me/Super_Nintendo_Entertainment_System_(SNES)_ROMs/List-All-Titles-By-Downloads-1/5"; // para testeo
    }
    casper.thenOpen(url);
});
casper.viewport(1024, 768);
casper.then(function () {
    casper.capture('st.png', {
        top: 0
        , left: 0
        , width: 1000
        , height: 1000
    });
});
casper.then(function () {
    /*Extrae los links de una lista */
    var urls = casper.evaluate(function () {
        var links = [];
        $('a.gamelist').each(function () {
            var url = $(this).attr('href');
            links.push(url);
        })
        return links;
    })
    var count = 0;
    var urlsClean = [];
    /* Recoge los enlaces que no se repitan y que no estén en Japones o en Beta*/
    casper.start().each(urls, function (self, link) {
        if (link.indexOf("Japan") == -1 && link.indexOf("Beta") == -1) {
            if (urlsClean.length > 0) {
                if (link.indexOf(urlsClean[urlsClean.length - 1].substr(0, 60)) == -1) {
                    urlsClean.push(link);
                }
            }
            else {
                urlsClean.push(link);
            }
        }
    });
    casper.then(function () {
        casper.echo('Total de enlaces recogidos: -> ' + urlsClean.length);
        casper.start().each(urlsClean, function (self, link) {
            self.thenOpen('https://www.emuparadise.me/' + link, function () {
                var descript = casper.evaluate(function () {
                    return $('div.description-text p').text();
                })
                var urls = casper.evaluate(function () {
                    var a = [];
                    $('div#slider ul li a.sc').each(function () {
                        a.push('http:' + $(this).attr('href'));
                    })
                    return a;
                })
                var title = splitTitle(this.getTitle());
                casper.then(function () {
                    if (urls != null) {
                        var imgs = [];
                        for (var i = 0; i < urls.length; i++) {
                            imgs.push(splitImg(urls[i]));
                        }
                        var bbdd = new Db(title, descript, imgs);
                        bbdd.add();
                        //Accede a cada url y descarga las imagenes
                        casper.each(urls, function (self, link) {
                            casper.thenOpen(link, function () {
                                //this.echo(splitImg(link));
                                this.download(link, 'downloads/' + 'gba' + '/' + splitImg(link), 'GET');
                            });
                        })
                    }
                    casper.then(function () {
                        casper.capture('st.png', {
                            top: 1000
                            , left: 0
                            , width: 1000
                            , height: 1000
                        });
                    });
                })
            });
        });
    })
});

function next() {
    casper.then(function () {
        this.click('a#next.mobile-hide');
    });
}

function splitTitle(title) {
    var exp = /(.*)(<.*)+(\|.*)+/g;
    title = title.replace(exp, '$1');
    return title;
}

function splitImg(img) {
    var exp = /(.*images\/)/g;
    img = img.replace(exp, '');
    return img;
}
casper.run();