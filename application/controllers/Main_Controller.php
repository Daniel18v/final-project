<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Main_Controller extends MY_Controller {

    public function __construct() {
        parent::__construct();
        $this->load->model('Main_Model');
        $this->load->helper('url');
    }

    /*
     * Index is the main page, and in it charges
     * all titles for show them in the view.
     */

    public function index() {
        $data['all'] = $this->Main_Model->get_all();
        $data['title'] = "FreeBird · Juega y a volar";
        if (empty($data['all'])) {
            show_404();
        }
        echo $this->templates->render('games::index', $data);
    }
    public function probando($p = FALSE) {
        echo "Hola tú";
        if ($p == 1) {
            echo " qué ase?";
        }
    }
    public function chat() {
        echo $this->templates->render('chat::chat');
    }
}
