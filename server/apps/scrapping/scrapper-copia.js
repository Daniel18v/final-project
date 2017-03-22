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
        url = "https://www.emuparadise.me/Nintendo_Gameboy_Advance_ROMs/Games-Starting-With-A/31"; // para testeo
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
                        a.push($(this).attr('href'));
                    })
                    return a;
                })
                var title = splitTitle(this.getTitle());
                casper.then(function () {
                    if (urls != null) {
                        for (var i = 0; i < urls.length; i++) {//hay que cambiar este for
                            var img = 'http:' + urls[i];
                            this.echo(img);
                            casper.thenOpen('http://r.mprd.se/', function () {
                                this.download(img, 'downloads/' + 'gba' + '/' + title + '-' + i + '.png', 'GET');
                            });
                        }
                        casper.then(function () {
                            casper.capture('st.png', {
                                top: 1000
                                , left: 0
                                , width: 1000
                                , height: 1000
                            });
                        });
                    }
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
/*
casper.then(function () {

        Extrae los links de una lista
        var gamesLinks = casper.evaluate(function () {
                var g = [];
                $('a.s18').each(function () {
                    g.push($(this).attr('href'));
                })
                return g;
            })
            Recorre la lista de links accede a cada uno de ellos y extrae la imagen y la descripción
        var imgs = [];
        var descripts = [];
        casper.start().each(gamesLinks, function (self, link) {
            self.thenOpen(link, function () {
                var descript = casper.evaluate(function () {
                    return $('p.mar_b10').text();
                })
                var urlImg = casper.evaluate(function () {
                    return x('//*[@id="tb926"]/a[@class="xXx"]/@href');
                })
                casper.echo('urlImg');
                descripts.push(descript);
                //imgs.push(urlImg);
            });
        });
        casper.echo('Imgs-> ' + descripts.length);
        var urlImgs = [];
        casper.start().each(imgs, function (self, link) {
            self.thenOpen(link, function () {
                var urlImg = casper.evaluate(function () {
                    return $('img.foro_img').attr('src');
                })
                urlImgs.push(urlImg);
            });
        });
        // Descarga las imágenes y les asigna el nombre del titulo.
        casper.then(function () {
            casper.start().each(urlImgs, function (self, link) {
                self.thenOpen(link, function () {
                    casper.download(link, splitImg(this.getTitle()));
                });
            });
        })
    })
    */
/*
casper.then(function () {
    var age = casper.evaluate(function () {
        if ($("#ageYear").val() === undefined) {
            age = 'null';
        } else {
            age = $("#ageYear").val('1994');
        }
        return age;
    });

    if(age != 'null') {
        this.click(x('//*[@id="agecheck_form"]/a/span'));
    }
})
casper.then(function() {
    var imgUrl = casper.evaluate(function() {
        return $('.game_header_image_ctn img').attr('src');
    })
    this.echo(imgUrl);
    var url = splitUrl(imgUrl);
    casper.start(url, function() {
        this.download(imgUrl, "logo.jpg");
    });

})
*/
//Captura el estado al finalizar
function splitTitle(title) {
    var exp = /(.*)(<.*)+(\|.*)+/g;
    title = title.replace(exp, '$1');
    return title;
}

function downloadImgs() {
    casper.echo(imgs.length);
    var count = 0;
}

function randomName() {
    return Math.round(Math.random() * 10000 - 0.5);
}
casper.run();
