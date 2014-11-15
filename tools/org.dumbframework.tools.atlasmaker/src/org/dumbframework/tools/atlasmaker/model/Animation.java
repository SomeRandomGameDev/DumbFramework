/**
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of the University of California, Berkeley nor the
 *   names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package org.dumbframework.tools.atlasmaker.model;

import java.util.Collections;
import java.util.Comparator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.eclipse.swt.graphics.Point;

/**
 * Animation. This is a quite complex object. It manipulates a set of key points on which sprites can be
 * anchored.
 * @author Stoned Xander
 */
public class Animation {

    /**
     * Key frame.
     * @author Stoned Xander
     */
    public static class Keyframe {
        // Define a key frame : Position, rotation, scale, vector (in case of bezier curve).
        /**
         * Time the keyframe occurs.
         */
        private long time;

        /**
         * Relative position.
         */
        private Point position;

        /**
         * Possible offset relative to the anchor.
         */
        private Point offset;

        /**
         * Rotation.
         */
        private double rotation;

        /**
         * Scale.
         */
        private double scale;

        /**
         * Frame subject.
         */
        private Sprite subject;

        /**
         * Subject anchor.
         */
        private String anchor;

        /**
         * Constructor.
         */
        public Keyframe() {
            // TODO.
        }

        /**
         * @return Provide frame time.
         */
        public long getTime() {
            return time;
        }
        
        
    }

    /**
     * Track.
     * @author Stoned Xander
     */
    public static class Track {

        /**
         * Time comparator to sort keyframe.
         */
        private static final Comparator<Keyframe> TIME_COMPARATOR = new Comparator<Keyframe>() {
            @Override
            public int compare(Keyframe o1, Keyframe o2) {
                int result = Long.compare(o1.getTime(), o2.getTime());
                return result;
            }
        };

        /**
         * Parent track (can be null).
         */
        private Track parent;

        /**
         * Sorted list of frames.
         */
        private List<Keyframe> frames = new LinkedList<Keyframe>();

        /**
         * @return Provide access to parent track.
         */
        public Track getParent() {
            return parent;
        }

        /**
         * Set the parent track.
         * @param track Parent track.
         */
        public void setParent(Track track) {
            parent = track;
        }

        /**
         * Add a frame.
         * @param frame Frame to add.
         */
        public void add(Keyframe frame) {
            if (!frames.contains(frame)) {
                frames.add(frame);
                sortByTime();
            }
        }

        /**
         * @param frame Frame to remove.
         */
        public void remove(Keyframe frame) {
            frames.remove(frame);
        }

        /**
         * Sort the frames by time.
         */
        public void sortByTime() {
            Collections.sort(frames, TIME_COMPARATOR);
        }
    }

    /**
     * Set of managed tracks.
     */
    private Map<String, Track> tracks = new TreeMap<String, Track>();

    /**
     * Constructor.
     */
    public Animation() {
    }
}
