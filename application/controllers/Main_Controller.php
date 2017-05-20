<?php

defined("BASEPATH") or exit("No direct script access allowed");

class Main_Controller extends MY_Controller {

    public function __construct() {
        parent::__construct();
        $this->load->model('Main_Model');
        $this->load->helper('url');
        $this->load->library('session');
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
        echo $this->templates->render('main::index', $data);
    }
    public function probando($p = FALSE) {
        echo "Hola tú";
        if ($p == 1) {
            echo " qué ase?";
        }
        echo $this->templates->render("main::prueba");
    }

    public function chat() {
        echo $this->templates->render('chat::chat');
    }

    public function emulator($p = FALSE, $roms = FALSE) {
        switch (strtolower($p)) {
            case "nes":
                echo $this->templates->render('emulator::jsNES');
                break;
            case "sms":
                echo $this->templates->render('emulator::jsSMS');
                break;
            default: show_404();
        }

        //$data = $this->Main_Model->get_id_roms($roms);
        //echo $this->templates->render('emulator::jsNES', $data);



    }

    public function games($p = FALSE) {
        echo $this->templates->render('main::games');
    }

    public function shop() {
        echo $this->templates->render('main::shop');
    }
}
