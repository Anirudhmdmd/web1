pipeline {
    agent any
    stages {
        stage('Checkout') {
            steps {
                git url: 'https://github.com/Anirudhmdmd/web1.git', branch: 'main'
            }
        }

        stage('Build Node Package') {
            steps {
                script {
                    // Create node_package directory if it doesn't exist
                    bat 'if not exist node_package mkdir node_package'

                    // List all files (including hidden) to dir_contents.txt
                    bat 'dir /a > dir_contents.txt'

                    // Print current directory path to current_dir.txt
                    bat 'echo Current directory: %cd% > current_dir.txt'

                    // Explicitly check for the existence of the 'label' file by echoing its path
                    bat '''
                    echo Checking if label file exists in the current directory...
                    if exist "label" (
                        echo label file found, copying to node_package
                        copy label node_package\\
                    ) else (
                        echo ERROR: label file not found in %cd%!
                        exit /b 1
                    )
                    '''
                }
            }
        }

        stage('Archive Package') {
            when {
                expression {
                    return fileExists('node_package')
                }
            }
            steps {
                // Archive the contents of node_package if it exists
                archiveArtifacts artifacts: 'node_package/**', fingerprint: true
            }
        }
    }

    post {
        always {
            // Clean workspace after build
            cleanWs()
        }
    }
}
