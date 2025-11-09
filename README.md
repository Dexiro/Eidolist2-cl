# Eidolist2-cl

Cleaned up version of the Eidolist2 project. Aiming to do the following;
- Get the dependencies and cmake files sorted out.
- Make sure the project can build and run correctly on Linux.
- Progress towards making the codebase tidier and more consistent. 
- Be more careful about any experimental or unfinished code that gets committed, branching the codebase if needed.

**Roadmap (no particular order);**

Improve the Asset Database (AssetLoader.h/cpp). 
- Currently all map textures that get generated are stored in a global Asset Database, I think this risks causing issues and has no real benefit. The Asset Database should only store textures that are loaded from the project files, such as Chipsets and Charsets, as these are likely to be reused many times (Eg. if a map has 100 events that all use the Debug charset, we don't want to load the texture for it 100 times).
- Assets stored in the database should be located using their file path. A checksum should be generated so that we're able to detect if an asset has been modified and may need to be reloaded.
- We need the ability to reload or remove assets for a particular map or a particular project.
- If a texture is updated in the database, we need a system in place to update any objects that are using that texture.
- If all of the objects that make use of a particular texture are deleted, then the texture should be removed from the database (Shared_ptr reference counting could be helpful here?). This is low priority though I don't think it'd be the end of the world to have a few unused textures stored in the DB. 

Re-implement multiple project support.

Implement project comparison features (this may be getting moved to a separate app).



