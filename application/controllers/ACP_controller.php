<?php
defined("BASEPATH") or exit("No direct script access allowed");

class ACP_controller extends MY_Controller {

    /**
     * Inherits of MY_Controller and charges the model for this controller.
     */
    public function __construct() {
        parent::__construct();
        $this->load->model('ACP_roms_model', 'roms');
        $this->load->model('ACP_users_model', 'users');
        $this->load->model('Blog_model', 'blog');
        $this->load->helper('ACP_helper');
    }

    /**
     * The main method that charges admin panel.
     */
    public function admin($section = FALSE) {
        echo $section ? $this->templates->render('main::admin_' . $section, ['sess' => $this]) :
            $this->templates->render('main::admin', ['sess' => $this]);
    }

    /**
     * This method is used for ajax query in the ACP.
     */
    public function roms_list() {
        $data = $this->roms->get_roms();
        echo json_encode($data);
    }

    public function rom_actions() {
        $action = $this->input->post('action');

        switch ($action) {
            case 'remove':
                $data = consult($this->input->post('data'));
                $this->roms->deleteRom($data);
                echo '{}';
                break;
            case 'edit':
                $data = consult($this->input->post('data'));
                $this->roms->updateRom($data);
                echo '{}';
                break;
            case 'create':
                $data = consult($this->input->post('data'));
                $this->roms->insertRom($data);
                echo '{}';
                break;
        }

    }
    public function user_list() {
        $data = $this->users->get_users();
        echo json_encode($data);
    }

    public function user_actions() {
        $action = $this->input->post('action');

        switch ($action) {
            case 'remove':
                $data = consult($this->input->post('data'));
                $this->users->deleteUser($data);
                echo '{}';
                break;
            case 'edit':
                $data = consult($this->input->post('data'));
                $this->users->updateUser($data);
                echo '{}';
                break;
            case 'create':
                $data = consult($this->input->post('data'));
                $this->users->insertUser($data);
                echo '{}';
                break;
        }

    }
    public function blog_list() {
        $data = $this->blog->get_blogs();
        echo json_encode($data);
    }
}
