<?

defined("BASEPATH") or exit("No direct script access allowed");

class Main_controller extends MY_Controller {

    public function __construct() {
        parent::__construct();
        $this->load->model('main_model');
        $this->load->library('encryption');
    }

    /*
     * Index is the main page, and in it charges
     * all titles for show them in the view.
     */

    public function index() {

        $data['all']   = $this->main_model->get_all();
        $data['title'] = "FreeBird · Juega y a volar";
        if (empty($data['all'])) {
            show_404();
        }
        echo $this->templates->render('main::index', $data);
    }


    public function probando($p = false) {
        echo "Hola tú";
        if ($p == 1) {
            echo " qué ase?";
        }
        echo $this->templates->render("main::prueba");
    }

    public function chat() {
        echo $this->templates->render('chat::chat');
    }
    public function shop() {
        echo $this->templates->render('main::shop');
    }
    public function social() {
        echo $this->templates->render('main::social');
    }
    public function news() {
        echo $this->templates->render('main::news');
    }
    public function blog() {
        echo $this->templates->render('main::blog');
    }
    public function forum() {
        echo $this->templates->render('main::forum');
    }
}
