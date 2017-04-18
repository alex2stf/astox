function ping(){
    mingui_notification("2 files compiled", "style.scss & desktop.css");
}

var localTrunk="/Users/alexstf/Desktop/cam4web/trunk";
var webPath="/web";
var warExploded="/Users/alexstf/Desktop/cam4web/trunk/out/artifacts/war_exploded/";

function localweb(from, toward){
    extapi_sassCompile( "/Users/alexstf/Desktop/cam4web/trunk/web/scss/style.scss",
        "/Users/alexstf/Desktop/cam4web/trunk/out/artifacts/war_exploded/css/stylesheets/style.css");
    extapi_sassCompile(  "/Users/alexstf/Desktop/cam4web/trunk/web/scss/desktop.scss",
        "/Users/alexstf/Desktop/cam4web/trunk/out/artifacts/war_exploded/css/stylesheets/desktop.css");



    fs_joinFilesWithConfig( "/Users/alexstf/Desktop/cam4web/trunk/web/js",
        "/Users/alexstf/Desktop/cam4web/trunk/out/artifacts/war_exploded/js",
        "/Users/alexstf/Desktop/cam4web/trunk/conf/frontend/config.json",
        "//#file ${index} ${file} compiled at ${timestamp} by cpp");


    fs_copyFile(localTrunk+webPath+"/js/Cam4Xtra.js", warExploded+"/js/Cam4Xtra.js");
    fs_copyFile(localTrunk+webPath+"/WEB-INF/ftl/javascriptFooter.ftl", warExploded+"/WEB-INF/ftl/javascriptFooter.ftl");
//
    if(from){
        fs_copyFile(from, toward);
    }


    mingui_notification("localweb runned", "style.scss & desktop.css");
}

function localwatch(){
    fs_watch("/Users/alexstf/Desktop/cam4web/trunk/web", function(updatedFile) {
            print(updatedFile);
            if (updatedFile.extension == "scss") {
                extapi_sassCompile("/Users/alexstf/Desktop/cam4web/trunk/web/scss/style.scss",
                    "/Users/alexstf/Desktop/cam4web/trunk/out/artifacts/war_exploded/css/stylesheets/style.css");
                extapi_sassCompile("/Users/alexstf/Desktop/cam4web/trunk/web/scss/desktop.scss",
                    "/Users/alexstf/Desktop/cam4web/trunk/out/artifacts/war_exploded/css/stylesheets/desktop.css");
            }

            if (updatedFile.extension == "js") {
                print("MERGE");
                fs_joinFilesWithConfig("/Users/alexstf/Desktop/cam4web/trunk/web/js",
                    "/Users/alexstf/Desktop/cam4web/trunk/out/artifacts/war_exploded/js",
                    "/Users/alexstf/Desktop/cam4web/trunk/conf/frontend/config.json",
                    "//#file ${index} ${file} compiled at ${timestamp} by cpp");
            }
        }
    );
}

var ipushLocalRoot="/Users/alexstf/Desktop/imagePush/imagePush/unstable/";
var ipushRemoteRoot="/home/dragosg/";


function ipushWatch(){
    print("ipushWatch");
    ssh2_connect("root", "smartx00x", "10.8.5.88");

    fs_watch(ipushLocalRoot, function(updatedFile) {
        var localPath = updatedFile.path.replace(ipushLocalRoot, "");
        localPath = localPath.replace("0.x-dev", "imagePush-0.5");
        var remotePath = ipushRemoteRoot + localPath;
        print(remotePath);
        mingui_notification(localPath, "sync done " + new Date());
        ssh2_scpWrite(updatedFile.path, remotePath);
    });
}

//localweb();
//ipushWatch();