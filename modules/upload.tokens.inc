<?php
// $Id$

/**
 * @file
 * Builds placeholder replacement tokens for uploaded files attached to nodes.
 */

/**
 * Implement hook_tokens().
 */
function upload_tokens($type, $tokens, $data, $language = NULL, $sanitize = TRUE) {
  $replacements = array();

  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];

    foreach ($tokens as $name => $original) {
      if ($name == 'upload') {
        $upload = array_shift($node->files);
        $replacements[$original] = file_create_url($upload->filepath);
      }
    }

    if (($upload_tokens = token_find_with_prefix($tokens, 'upload')) && !empty($node->files) && $upload = array_shift($node->files)) {
      $replacements += module_invoke_all('tokens', 'file', $upload_tokens, array('file' => $upload), $language, $sanitize);
    }
  }

  return $replacements;
}

function upload_token_info() {
  $results = array();

  $tokens['upload'] = array(
    'name' => t('File attachment'),
    'description' => t('The first file attached to a node, if one exists.'),
    'references' => 'file',
  );
  
  $results['tokens']['node'] = $tokens;
  return $results;
}