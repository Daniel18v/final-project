<?php

namespace App\Http\Requests;

use Illuminate\Foundation\Http\FormRequest;

class ChapterCreateRequest extends FormRequest
{
    /**
     * Determine if the user is authorized to make this request.
     *
     * @return bool
     */
    public function authorize()
    {
        return true;
    }

    /**
     * Get the validation rules that apply to the request.
     *
     * @return array
     */
    public function rules()
    {
        return [
            'title' => 'required|max:255',
            'chap_number' => 'required|numeric|min:1|max:999',
            'slug' => 'required|max:255',
            'excerpt' => 'required',
            'online' => 'numeric|min:0|max:1',
            'active' => 'numeric|min:0|max:1'
        ];
    }
}
