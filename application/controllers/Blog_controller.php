<?php
defined("BASEPATH") or exit("No direct script access allowed");

class Blog_controller extends MY_Controller
{

    /**
     * Inherits of MY_Controller and charges the model for this controller.
     */
    public function __construct()
    {
        parent::__construct();
        $this->load->model('Blog_model', 'mod');
    }

    /**
     * The main method that charges admin panel.
     */
    public function blog($slug = FALSE)
    {
        $query = $this->mod->get_blog($slug);
        empty(!$query) ? $blog = $query[0] : show_404();
        $comments = $this->mod->get_comments($slug);
        echo $this->templates->render('main::blog', ['sess' => $this, 'blog' => $blog, 'comments'=> $comments]);
    }

    public function blog_comment($slug = FALSE)
    {
        $data = $this->input->post();
        $data['user'] = $this->session->user;
        $data['user_avatar'] = $this->session->avatar;
        $this->mod->insert_commentary($data, $slug);
    }

    public function blog_actions($action = FALSE)
    {
        switch ($action) {
            case 'insert':
                $this->mod->insertData($this->input->post());
                echo 200;
                breack;

        }

    }

}
